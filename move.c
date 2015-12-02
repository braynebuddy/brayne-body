/*
  move.c

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
  2015-12-01   3.1  Clean up more old code. Rename to move.c

*/
#include <math.h>                             // Needed for atan2() and M_PI

#include "simpletools.h"                      // Include simpletools header
#include "abdrive.h"                          // Include abdrive header
#include "botports.h"                         // Ports in use for the ActivityBot
#include "move.h"                             // Move the ActivityBot around
#include "transforms.h"                       // Coordinate transforms
#include "slam.h"                             // Localization, Mapping, and Coordinates

volatile int maxSpeed = 128;   // ticks/s
volatile int minSpeed = 0;     // ticks/s
volatile int botSpeed;         // ticks/s
volatile int leftSpeed;        // ticks/s
volatile int rightSpeed;       // ticks/s

// ----------------------------------------------
// Local helper functions.
// ----------------------------------------------

int _turnTicks(float a)
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

int _setDelta(int deltaSpeed)
{
  // Set the right and left wheel speeds to achieve a deltaSpeed
  // difference with no change in the current speed. If that's not
  // possible, reduce the speed to achieve deltaSpeed specified.
  // Function returns the speed achieved at the requested delta

  // Sanity checks on requested deltaSpeed
  if (deltaSpeed < -2*maxSpeed) deltaSpeed = -2 * maxSpeed;
  if (deltaSpeed > 2*maxSpeed) deltaSpeed = 2 * maxSpeed;

  // Figure out the limits on average speed given requested delta
  int hiSpeed = (deltaSpeed>0) ? (maxSpeed-deltaSpeed/2) : (maxSpeed+deltaSpeed/2);

  // Reduce speed if necessary
  if (botSpeed > hiSpeed) botSpeed = hiSpeed;
  if (botSpeed < -hiSpeed) botSpeed = -hiSpeed;

  rightSpeed = botSpeed + deltaSpeed/2;
  leftSpeed = rightSpeed - deltaSpeed;

  // Final check to make sure left wheel speed is within limits 
  if (leftSpeed < -maxSpeed) leftSpeed = -maxSpeed;
  if (leftSpeed > maxSpeed) leftSpeed = maxSpeed;

  // Set the bot to the requested deltaSpeed
  botSpeed = (rightSpeed + leftSpeed)/2;
  return (botSpeed);
}

int _setSpeed(int s)
{
  // Set the left and right wheel speeds to achieve an
  // overall speed of "s" ticks/sec while maintaining
  // the current right-left difference if possible. If
  // that's not possible, reduce the wheel speed difference
  // to achieve the overall speed.
  // Function returns the delta speed achieved at the requested speed

  // Sanity checks on requested speed
  if (s < -maxSpeed) s = -maxSpeed;
  if (s > 2*maxSpeed) s = maxSpeed;

  // Figure out the limits on delta speed given requested speed
  int hiDelta = (s>0) ? 2*(maxSpeed-s) : 2*(maxSpeed+s);

  // Reduce delta speed if necessary
  int delta = rightSpeed - leftSpeed;
  if (delta > hiDelta) delta = hiDelta;
  if (delta < -hiDelta) delta = -hiDelta;

  rightSpeed = s + delta/2;
  leftSpeed = rightSpeed - delta;
  
  // Final check to make sure left wheel speed is within limits 
  if (leftSpeed < -maxSpeed) leftSpeed = -maxSpeed;
  if (leftSpeed > maxSpeed) leftSpeed = maxSpeed;

  // Clean up variables and return delta achieved at requested speed.
  botSpeed = (leftSpeed + rightSpeed)/2;
  return (delta);
}

void _driveSpeed(int left, int right) {
  drive_speed(left+LSERVOBIAS, right+RSERVOBIAS);
}

void _driveGoto(int left, int right) {
  drive_goto(left, right);
}

// ----------------------------------------------
// Functions intended to be called from outside.
// ----------------------------------------------

void botStop()
{
  // Stop the ActivityBot by setting the wheel speeds to zero
  leftSpeed = 0;
  rightSpeed = 0;
  botSpeed = 0;
  drive_speed(0,0);
}

void botTurn(float a)
{
  // Stop the ActivityBot and turn the requested angle (in radians).
  // Assume we want half of the turn on each wheel.

  int l_ticks = 0;
  int r_ticks = 0;
  int turn_ticks = 0;

  drive_speed(0,0); // Stop bot
  leftSpeed = 0;
  rightSpeed = 0;
  botSpeed = 0;

  turn_ticks = _turnTicks(a);
  r_ticks = turn_ticks / 2;
  l_ticks = r_ticks - turn_ticks;

  drive_goto(l_ticks, r_ticks); // Turn in place

}

void botMove(int mm)
{
  // Stop the ActivityBot and move the requested distance (in mm).

  // Encoder ticks are 3.25 mm/tick, so 13 cm = 4 ticks
  int ticks = mm * 4 / 13;

  drive_speed(0,0); // Stop bot
  leftSpeed = 0;
  rightSpeed = 0;
  botSpeed = 0;

  drive_goto(ticks, ticks);
}

void botSetMaxSpeed(int s)
{
  // Set maximum speed limit to "s" mm/s
  // Encoder ticks are 3.25 mm/tick, so 13 mm = 4 ticks
  // ActivityBot maximum wheel speed is 128 ticks/s = 416 mm/s
  if ( s > 416 ) s = 416;
  if ( s < -416 ) s = -416;
  maxSpeed = s * 4/13;

  //print("botSetMaxSpeed: s = %d mm/sec (%d ticks/s) %c\n", s, s*4/13, CLREOL);
  drive_setMaxSpeed(s * 4/13);
}

void botSetRampRate(int r)
{
  // Encoder ticks are 3.25 mm/tick, so 13 mm = 4 ticks

  //print("botSetRampRate: r = %d cm/sec/sec %c\n", r, CLREOL);
  drive_setRampStep(r*4/13);
}

void botSetSpeed(float vel)
{
  // Set the current speed to "vel" mm/s
  // Encoder ticks are 3.25 mm/tick, so 13 mm = 4 ticks
  int s;
  if (vel < 3.25) vel = 0.0;
  s = round(vel * 4.0/13.0);
  _setSpeed(s); // Calc wheel speeds, modify delta speed if necessary
  drive_speed(leftSpeed, rightSpeed);
}
  
void botSetRotation(float omega)
{
  // Set the ActivityBot's rate of rotation to omega radians/sec
  // The ActivityBot width is 105.8 mm, or 32.554 ticks
  // omega = (rightSpeed - leftSpeed) / botWidth
  // so,
  // rightSpeed - leftSpeed = deltaSpeed = omega * botWidth

  float L = 105.8;  // Wheel spacing = 105.8 mm
  float R = 33.1;   // Wheel radius = 33.1 mm

  _setDelta(round(omega * L / 3.25)); // Calc wheel speeds, modify avg speed if necessary

  // Set the bot to the requested angular velocity
  drive_speed(leftSpeed, rightSpeed);
}

void botSetVW(float vel, float omega)
{
  // Set wheel speeds so that ActivityBot moves at linear velocity (mm/s)
  // and angular velocity omega (rad/s). If both are not possible, angular
  // velocity takes precedence.

  float leftV;
  float rightV;

  if (vel < 3.25) vel = 0.0;
  botSpeed = round(vel/3.25); // Set target speed
  botSetRotation(omega);  // Set wheel speeds to achieve target omega
}

float pid_omega(float xy[2])
{
  // Calculate the angular rotation of the ActivityBot based on a target (x,y)
  // position for the bot. The target is in bot coordinate frame, so current bot
  // pose is (x,y) = (0,0) theta = 0 by definition.

  // PID tuning parameters
  float Kc = -2.0;
  float Ki = -0.0;
  float Kd = 0.0;
  // cyle-to-cycle errors
  static float e_sum = 0.0;
  static float old_e = 0.0;
  // current errors
  float e;
  float e_dot;
  float omega;

  // The error is the difference between the current bot heading and the
  // direction to the target.
  e = 0.0 - atan2(xy[1], xy[0]);

  // Make sure |e| < PI to avoid instability (+PI same as -PI)
  if (e < -M_PI) e = e + 2.0*M_PI;
  if (e >  M_PI) e = e - 2.0*M_PI;

  e_dot = (e - old_e)/0.1;
  e_sum += e * 0.1;

  omega = Kc * e + Ki * e_sum + Kd * e_dot;
  old_e = e;

  return omega;
}

void goTowardPose(float goal[3])
{
  // Move toward a position in world coordinate frame

  float goalW[] = {goal[0], goal[1]};   // Goal in world coordinate frame (gx,gy)
  float goalB[2];                   // Goal in Bot coordinate frame
  float goalD = sqrt(pow(goalW[0]-botP[0],2.0) + pow(goalW[1]-botP[1],2.0));

  float velocity;                   // Bot velocity in mm/s
  float omega;                      // Bot angular rotation in 1/s

  if(goalD > 10.0)
  {
    // calculate pose of goal in bot coordinate frame
    aTb_inv(goalW, goalB, botP);
    // calculate angular velocity, omega
    omega = pid_omega(goalB);
    // calculate desired velocity based on distance from goal
    velocity = goalD<200.0 ? goalD : 200.0;
    velocity = velocity<33.0 ? 33.0 : velocity;
    // set velocity and omega
    botSetVW(velocity, omega);
  } else {
    // We are "close" to goal (x,y), so turn to requested theta
    botTurn(goal[2] - botP[2]);
    
  }
}
