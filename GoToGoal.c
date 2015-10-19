/*
  GoToGoal.c

  Starting at (0,0,0) go to a specified (gx,gy,gtheta) using the pid_theta and
  gotoVW functions in movement.c

  ------------------------------------------------------------------------------
  Copyright 2015 Robert B. Hawkins
  Distributed under the MIT License
  (see accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
  ------------------------------------------------------------------------------

*/
#include "simpletools.h"                      // Include simple tools

#include "botports.h"                         // Ports in use for the ActivityBot
#include "sensors.h"                          // Manage sensors in use on the ActivityBot
#include "movement.h"                         // Move the ActivityBot around
#include "transforms.h"                       // Coordinate frame transforms

#define round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

int main()                                    // Main function
{
  botP[0] = 0.0;     // Bot position and orientation in world coordinate frame (x,y)
  botP[1] = 0.0;
  botTheta = 0.0;

  float goalW[] = {500.0, 400.0};   // Goal in world coordinate frame (gx,gy)
  float goalB[2];                   // Goal in Bot coordinate frame
  float goalD = sqrt(pow(goalW[0]-botP[0],2.0) + pow(goalW[1]-botP[1],2.0));

  float velocity;                   // Bot velocity in mm/s
  float omega;                      // Bot angular rotation in 1/s

  int cycle = 0;

  // Send out startup announcement
  freqout(4, 500, 3000);                      // Speaker tone: 0.5 s @ 3 kHz, 0.25 s @ 3.5 kHz
  freqout(4, 250, 3500);

  // Set up some variables we will need
  botSetMaxSpeed(128);
  botSetRampRate(12);
  pingAngle(0);
 
  while(goalD > 10.0)
  {
    print("%c", HOME);
    print("Cycle: %d%c\n", cycle, CLREOL);

    // update current pose in world coordinate frame
    updatePose();
    goalD = sqrt(pow(goalW[0]-botP[0],2.0) + pow(goalW[1]-botP[1],2.0));
    print("bot at (%f,%f), theta: %f %c\n", botP[0], botP[1], botTheta, CLREOL);

    // calculate pose of goal in bot coordinate frame
    aTb_inv(goalW, goalB, botP[0], botP[1], botTheta);
    print("goal(W) at (%f,%f), theta: %f %c\n", goalW[0], goalW[1], atan2(goalW[1],goalW[0]), CLREOL);
    print("goal(B) at (%f,%f), theta: %f %c\n", goalB[0], goalB[1], atan2(goalB[1],goalB[0]), CLREOL);

    // calculate omega
    omega = pid_omega(goalB);
    print("omega = %f%c\n", omega, CLREOL);

    // calculate maximum velocity for this omega
    velocity = goalD<200.0 ? goalD : 200.0;
    velocity = velocity<33.0 ? 33.0 : velocity;
    print("velocity = %f%c\n", velocity, CLREOL);

    // set velocity and omega
    botSetVW(velocity, omega);

    print("goalD =%f%c\n", goalD, CLREOL);
    print("Pausing...%c\n", CLREOL);
    pause(100);
    cycle += 1;
  } // End of while()

  print("botSpeed =%f (%f, %f)%c\n", botSpeed, leftSpeed, rightSpeed, CLREOL);

  print("Stopping...%c\n", CLREOL);
  botSetSpeed(0.0);
  print("botSpeed =%d (%d, %d)%c\n", botSpeed, leftSpeed, rightSpeed, CLREOL);
  print("Stopped%c\n", CLREOL);
  botStop();
  print("botSpeed =%d (%d, %d)%c\n", botSpeed, leftSpeed, rightSpeed, CLREOL);
  botMove(0);
  print("botSpeed =%d (%d, %d)%c\n", botSpeed, leftSpeed, rightSpeed, CLREOL);
} // End of main()
