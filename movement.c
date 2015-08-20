/*
  movement.c

  Cause the ActivityBot to move in a variety of ways.

  ------------------------------------------------------------------------------
  Copyright 2015 Robert B. Hawkins
  Distributed under the MIT License
  (see accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
  ------------------------------------------------------------------------------

  Date        Ver   Comments
  ==========  ====  ==================================================
  2015-07-17   1.0  Separate turnAngle() into it's own file, correct 
                    coordinate frames to us bot reference.
  2015-07-18   2.0  Start from turnAngle() function
  2015-08-02   2.1  Convert speed from ticks/sec to cm/sec
  2015-08-17   2.2  Convert to mm/s, add control functions

*/
#include <math.h>                             // Needed for atan2()

#include "simpletools.h"                      // Include simpletools header
#include "abdrive.h"                          // Include abdrive header
#include "botports.h"                         // Ports in use for the ActivityBot
#include "movement.h"                         // Move the ActivityBot around

#define round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif 

volatile int botMaxSpeed;   // ticks/s
volatile int botSpeed;      // ticks/s
volatile int leftSpeed;     // ticks/s
volatile int rightSpeed;    // ticks/s

int turnTicks(int a)
{
  // Calculate the number of extra ticks (+ or -) needed to cause the ActivityBot 
  // to turn the requested number of degrees. Assume incoming angle is in bot 
  // coordinate frame (i.e. 0 degrees is directly ahead, angles increase 
  // counter-clockwise).

  // For the ActivityBot, Turning radius is 105.8 mm, so turning circle
  // in 664.761 mm. Encoder ticks are 3.25 mm/tick, so 204.542 ticks will
  // produce 360 degrees of turning.

  int t;

  // adjust angle to be -180 to + 180 range
  //a = 10 * a; // convert to tenths of a degree
  while (a > 180)
    a = a - 360;
  while (a < -180)
    a = a + 360;

  // calculate right-wheel delta ticks to turn by angle 'a'
  //t = (10*a + 88) / 176; // 1 tick = 360/204.542 = 1.76 degrees. This formula has < 1 deg error.
  t = (int)((float)a / 1.76 + 0.5);

  return (t);
}

void botTurnAngle(int a)
{
  // Stop the ActivityBot and turn the requested number of degrees.
  // Assume we want half of the turn on each wheel.

  int l_ticks = 0;
  int r_ticks = 0;
  int turn_ticks = 0;

  drive_ramp(0,0); // Stop bot
  botSpeed = 0;

  turn_ticks = turnTicks(a);
  r_ticks = turn_ticks / 2;
  l_ticks = r_ticks - turn_ticks;

  drive_goto(l_ticks, r_ticks); // Turn in place
}

void botRotation(int omega)
{
  // Set the ActivityBot's rate of rotation to omega degrees/sec
  // The ActivityBot width is 105.8 mm, or 32.554 ticks
  // omega = (rightSpeed - leftSpeed) / botWidth
  // so,
  // rightSpeed - leftSpeed = deltaSpeed = omega * botWidth
  // rightSpeed = botSpeed + deltaSpeed / 2
  // leftSpeed =  rightSpeed - deltaSpeed

  // convert to radians
  float omega_r = (float)omega / 57.2957795;
  int deltaSpeed = (int)(omega_r * 32.544 + 0.5);
  rightSpeed = botSpeed + deltaSpeed/2;
  leftSpeed = rightSpeed - deltaSpeed;
  botSpeed = (rightSpeed + leftSpeed)/2;

  drive_ramp(leftSpeed, rightSpeed);
}

void botTurnHeading(int angle, int duration)
{
  int ticks = 0;
  int t;
  int leftTicks;
  int rightTicks;
  int turning = 0;

  if (duration < 1) duration = 1;
  if (angle != 0) turning = 1;
  ticks = turnTicks(angle);
  t = ticks / duration;
  //print("movement: angle = %d, ticks = %d, t = %d %c\n", angle, ticks, t, CLREOL);

  drive_getTicks(&leftTicks, &rightTicks);
  //print("movement: rightTicks = %d, leftTicks = %d %c\n", leftTicks, rightTicks, CLREOL);
  ticks = ticks + rightTicks - leftTicks;
  //print("movement: leftTicks = %d, rightTicks = %d, R-L = %d, ticks = %d %c\n",
  //         leftTicks, rightTicks, rightTicks-leftTicks, ticks, CLREOL);
  drive_ramp(botSpeed - t/2, botSpeed + t/2);
  while (turning)
  {
    drive_getTicks(&leftTicks, &rightTicks);
    turning = 0;
    if ((ticks > 0) && ((rightTicks-leftTicks) < ticks)) turning = 1;
    if ((ticks < 0) && ((rightTicks-leftTicks) > ticks)) turning = 1;
    //print("movement: leftTicks = %d, rightTicks = %d, R-L = %d, ticks = %d %c\n",
    //       leftTicks, rightTicks, rightTicks-leftTicks, ticks, CLREOL);
  }
  //print("movement: Return to normal speed. L=%d,R=%d,R-L=%d%c\n",leftTicks,rightTicks,rightTicks-leftTicks,CLREOL);
  drive_ramp(botSpeed, botSpeed);
  //scan("%d",&test);
}

void botSetMaxSpeed(int s)
{
  // Encoder ticks are 3.25 mm/tick, so 13 mm = 4 ticks
  // Min speed is 1 tick/sec = 3 mm/sec. Max speed is 128 ticks/sec = 417 mm/sec
  //print("botSetMaxSpeed: s = %d cm/sec %c\n", s, CLREOL);
  botMaxSpeed = s*4/13;
  drive_setMaxSpeed(s*4/13);
}  

void botSetRampRate(int r)
{
  // Encoder ticks are 3.25 mm/tick, so 13 mm = 4 ticks
  //print("botSetRampRate: r = %d cm/sec/sec %c\n", r, CLREOL);
  drive_setRampStep(r*4/13);
}

void botSetDeltaSpeed(int d)
{
  // Encoder ticks are 3.25 mm/tick, so 13 cm = 4 ticks
  d = d * 4/13;
  botSpeed = (rightSpeed + leftSpeed)/2;
  rightSpeed = botSpeed + d/2;
  leftSpeed = rightSpeed - d;
  botSpeed = (rightSpeed + leftSpeed)/2;

  drive_ramp(leftSpeed, rightSpeed);
}
void botSetSpeed(int s)
{
  //print("botSetSpeed: s = %d ticks/sec%c\n", s, CLREOL);
  botSpeed = s;
  leftSpeed = s;
  rightSpeed = s;
  
  drive_speed(leftSpeed, rightSpeed);
}

void botMove(int mm)
{
  // Encoder ticks are 3.25 mm/tick, so 13 cm = 4 ticks
  int ticks = mm * 4 / 13;

  //ticks = (int)((float)mm/3.25 + 0.5);

  drive_goto(ticks, ticks);
}

void botToPose(int x, int y, int theta)
{

}

void botLeftTurn(int distance, int theta)
{
  distance = distance * 10000 / 325;
  rightSpeed = botSpeed *(distance - 1539) / (distance - 3077);
  leftSpeed = botSpeed * (distance - 4615) / (distance - 3077);
  botSpeed = (rightSpeed + leftSpeed) / 2;
}    

void botSetVW(float vel, float omega)
{
  print("botSetVW:vel = %f omega = %f%c\n", vel, omega, CLREOL);
  // Set wheel speeds so that ActivityBot moves at linear velocity (mm/s)
  // and angular velocity omega (rad/s)
  float L = 105.8;  // Wheel spacing = 105.8 mm
  float R = 33.1;   // Wheel radius = 33.1 mm
  float leftV;
  float rightV;

  //if (vel < 3.25) vel = 0.0;
  rightV = vel + (omega*L/2.0); // vR [=] mm/sec
  leftV = vel - (omega*L/2.0);  // vL [=] mm/sec
  print ("botVelocity = %f (%f,%f)%c\n", vel, leftV, rightV, CLREOL);

  rightSpeed = round(rightV / 3.25); // 1 tick = 3.25 mm
  leftSpeed = round(leftV / 3.25);   // 1 tick = 3.25 mm

  botSpeed = (rightSpeed + leftSpeed) / 2; 
  print ("botSpeed = %d (%d,%d)%c\n", botSpeed, leftSpeed, rightSpeed, CLREOL);

  drive_speed(leftSpeed, rightSpeed); 
  print("botSetVW: done%c\n", CLREOL);
}

float pid_omega(float xy[2])
{
  print ("xy = (%f,%f)%c\n", xy[0], xy[1], CLREOL);
  // The goal (x,y) is in bot coordinate frame, so current bot
  // theta = 0 by definition.
  float Kc = -6.0;
  float Ki = -0.5;
  float Kd = 0.0;

  static float e_sum = 0.0;
  static float old_e = 0.0;

  float e;
  float e_dot;
  float omega;

  // Make sure |e| < PI
  e = 0.0 - atan2(xy[1], xy[0]);
  if (e < -M_PI) e = e + 2.0*M_PI;
  if (e >  M_PI) e = e - 2.0*M_PI;

  e_dot = (e - old_e)/0.1;
  old_e = e;

  e_sum += e * 0.1;

  print ("e = %f, e_sum = %f, e_dot = %f%c\n", e, e_sum, e_dot, CLREOL);
  omega = Kc * e + Ki * e_sum + Kd * e_dot;

  return omega;
} 
