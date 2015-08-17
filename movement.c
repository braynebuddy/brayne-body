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

*/
#include "simpletools.h"                      // Include simpletools header
#include "abdrive.h"                          // Include abdrive header
#include <math.h>                             // Needed for atan2()

#define round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif 

volatile int botSpeed;
volatile int leftSpeed;
volatile int rightSpeed;

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
  // Encoder ticks are 3.25 mm/tick, so 13 cm = 40 ticks
  // Min speed is 1 tick/sec = 3 mm/sec. Max speed is 128 ticks/sec = 417 mm/sec
  //print("botSetMaxSpeed: s = %d cm/sec %c\n", s, CLREOL);
  drive_setMaxSpeed(s*40/13);
}  

void botSetRampRate(int r)
{
  // Encoder ticks are 3.25 mm/tick, so 13 cm = 40 ticks
  //print("botSetRampRate: r = %d cm/sec/sec %c\n", r, CLREOL);
  drive_setRampStep(r*40/13);
}

void botSetDeltaSpeed(int d)
{
  // Encoder ticks are 3.25 mm/tick, so 13 cm = 40 ticks
  d = d * 40/13;
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
  
  drive_ramp(leftSpeed, rightSpeed);
}

void botMove(int cm)
{
  // Encoder ticks are 3.25 mm/tick, so 13 cm = 40 ticks
  int ticks = cm * 40 / 13;

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

void botSetVW(float velocity, float omega)
{
  // Set wheel speeds so that ActivityBot moves at linear velocity (mm/s)
  // and angular velocity omega (rad/s)
  float L = 105.8;  // Wheel spacing = 105.8 mm
  float R = 33.1;   // Wheel radius = 33.1 mm
  float leftVelocity;
  float rightVelocity;

  if (velocity < 3.25) velocity = 0.0;
  rightVelocity = (2.0*velocity + omega*L) / (2.0 * R); // vR [=] cm/sec
  rightSpeed = round(rightVelocity / 3.25); // 1 tick = 3.25 mm

  leftVelocity = (2.0*velocity - omega*L) / (2.0 * R);  // vL [=] cm/sec
  leftSpeed = round(leftVelocity / 3.25); // 1 tick = 3.25 mm
  botSpeed = (rightSpeed + leftSpeed) / 2; 

  drive_ramp(leftSpeed, rightSpeed); 
}

float pid_omega(float deltaX, float deltaY, float omega0)
{
  float Kc = -0.5;
  float Ki = 0.0;
  float Kd = 0.0;

  static float e_sum = 0.0;
  static float old_e = 0.0;

  float e;
  float e_dot;
  float delta_omega;

  e = omega0 - atan2(deltaX, deltaY);
  if (e < -M_PI) e = e + 2.0*M_PI;
  if (e >  M_PI) e = e - 2.0*M_PI;

  e_dot = e - old_e;
  old_e = e;

  e_sum += e;

  delta_omega = Kc * e + Ki * e_sum + Kd * e_dot;

  return delta_omega;
} 