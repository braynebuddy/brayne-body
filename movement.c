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
  2015-08-29   3.0  Remove old non-functional routines, standardize units
                    to be mm for distance, radians for angles, ticks for 
                    wheel speeds

*/
#include <math.h>                             // Needed for atan2() and M_PI

#include "simpletools.h"                      // Include simpletools header
#include "abdrive.h"                          // Include abdrive header
#include "botports.h"                         // Ports in use for the ActivityBot
#include "movement.h"                         // Move the ActivityBot around

#define round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

// PI = 3.1415926535897932384626433832795 (probably defined in <math.h>)
#ifndef M_PI
#define M_PI  3.141592654
#endif 
#define M_2PI (2.0*M_PI)

volatile int botMaxSpeed = 128;   // ticks/s
volatile int botMinSpeed = 10;   // ticks/s
volatile int botSpeed;      // ticks/s
volatile int leftSpeed;     // ticks/s
volatile int rightSpeed;    // ticks/s

int turnTicks(float a)
{
  // Calculate the number of extra ticks (+ or -) needed to cause the ActivityBot 
  // to turn by the requested angle. Assume incoming angle is in bot 
  // coordinate frame (i.e. 0 radians is directly ahead, angles increase 
  // counter-clockwise).

  // For the ActivityBot, Turning radius is 105.8 mm, so turning circle
  // in 664.761 mm. Encoder ticks are 3.25 mm/tick, so 204.542 ticks will
  // produce 2*PI radians of turning.

  int t;

  // adjust angle to be -PI to +PI range
  while (a > M_PI)
    a = a - M_2PI;

  while (a < -M_PI)
    a = a + M_2PI;

  // calculate right-wheel delta ticks to turn by angle 'a'
  t = round(a * 204.542 / M_2PI);

  return (t);
}

void botTurnAngle(float a)
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

void botSetMaxSpeed(int s)
{
  // Set maximum speed limit to "s" mm/s
  // Encoder ticks are 3.25 mm/tick, so 13 mm = 4 ticks
  // ActivityBot maximum wheel speed is 128 ticks/s = 416 mm/s
  if ( s > 416 ) s = 416;
  if ( s < -416 ) s = -416;
  botMaxSpeed = s * 4/13;

  //print("botSetMaxSpeed: s = %d mm/sec (%d ticks/s) %c\n", s, s*4/13, CLREOL);
  drive_setMaxSpeed(s * 4/13);
}

void botSetRampRate(int r)
{
  // Encoder ticks are 3.25 mm/tick, so 13 mm = 4 ticks

  //print("botSetRampRate: r = %d cm/sec/sec %c\n", r, CLREOL);
  drive_setRampStep(r*4/13);
}

void botSetDeltaSpeed(int deltaSpeed)
{
  // Set the right and left wheel speeds to achieve a deltaSpeed
  // difference with no change in the current speed. If that's not
  // possible, reduce the speed to achieve deltaSpeed specified.

  if (deltaSpeed < -2*maxSpeed) deltaSpeed = -2 * maxSpeed;
  if (deltaSpeed > 2*maxSpeed) deltaSpeed = 2 * maxSpeed;

  rightSpeed = botSpeed + deltaSpeed/2;
  leftSpeed = rightSpeed - deltaSpeed;

  // TODO Resume here @@

  if (rightSpeed < -maxSpeed) rightSpeed = -maxSpeed;
  if (rightSpeed > maxSpeed) rightSpeed = maxSpeed;

  botSpeed = (rightSpeed + leftSpeed)/2;
  drive_speed(leftSpeed, rightSpeed);
}

void botSetSpeed(int s)
{
  // Set the left and right wheel speeds to achieve an
  // overall speed of "s" ticks/sec while maintaining
  // the current right-left difference if possible. If
  // that's not possible, reduce the wheel speed difference
  // to achieve the overall speed.

  int current = (leftSpeed + rightSpeed)/2;
  int currentDelta = rightSpeed - leftSpeed;

  if (s < -maxSpeed) s = -maxSpeed;
  if (s > maxSpeed) s = maxSpeed;

  rightSpeed += s - current;
  leftSpeed += s - current;
  
  if (rightSpeed < -maxSpeed) {
    rightSpeed = -maxSpeed;
    leftSpeed = 2 * s - rightSpeed;
  }
  if (rightSpeed > maxSpeed) {
    rightSpeed = maxSpeed;
    leftSpeed = 2 * s - rightSpeed;
  }
  if (leftSpeed < -maxSpeed) {
    leftSpeed = -maxSpeed;
    rightSpeed = 2 * s - leftSpeed;
  }
  if (leftSpeed > maxSpeed) {
    leftSpeed = maxSpeed;
    rightSpeed = 2 * s - leftSpeed;
  }
  
  botSpeed = (leftSpeed + rightSpeed)/2;
  drive_speed(leftSpeed, rightSpeed);
}

void botRotation(float omega)
{
  // Set the ActivityBot's rate of rotation to omega radians/sec
  // The ActivityBot width is 105.8 mm, or 32.554 ticks
  // omega = (rightSpeed - leftSpeed) / botWidth
  // so,
  // rightSpeed - leftSpeed = deltaSpeed = omega * botWidth

  float L = 105.8;  // Wheel spacing = 105.8 mm
  float R = 33.1;   // Wheel radius = 33.1 mm

  botSetDeltaSpeed(round(omega * L / 3.25));
}

void botMove(int mm)
{
  // Encoder ticks are 3.25 mm/tick, so 13 cm = 4 ticks
  int ticks = mm * 4 / 13;

  //ticks = (int)((float)mm/3.25 + 0.5);

  drive_goto(ticks, ticks);
}

void botSetVW(float vel, float omega)
{
  // Set wheel speeds so that ActivityBot moves at linear velocity (mm/s)
  // and angular velocity omega (rad/s)

  float leftV;
  float rightV;

  if (vel < 3.25) vel = 0.0;
  botSpeed = round(vel/3.25);
  botRotation(omega);
}

float pid_omega(float xy[2])
{
  // The goal (x,y) is in bot coordinate frame, so current bot
  // theta = 0 by definition.
  float Kc = -6.0;
  float Ki = -0.0;
  float Kd = 0.0;

  static float e_sum = 0.0;
  static float old_e = 0.0;

  float e;
  float e_dot;
  float omega;

  //print ("xy = (%f,%f)%c\n", xy[0], xy[1], CLREOL);

  // Make sure |e| < PI
  e = 0.0 - atan2(xy[1], xy[0]);
  if (e < -M_PI) e = e + 2.0*M_PI;
  if (e >  M_PI) e = e - 2.0*M_PI;

  e_dot = (e - old_e)/0.1;
  old_e = e;

  e_sum += e * 0.1;

  //print ("e = %f, e_sum = %f, e_dot = %f%c\n", e, e_sum, e_dot, CLREOL);
  omega = Kc * e + Ki * e_sum + Kd * e_dot;

  return omega;
} 
