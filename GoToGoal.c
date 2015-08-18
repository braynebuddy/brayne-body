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

#define round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

int main()                                    // Main function
{
  float x = 0.0;        // Pose in world coordinate frame (x,y,theta)
  float y = 0.0;
  float theta = 0.0;

  float gx = -100.0;    // Goal in world coordinate frame (gx,gy,gtheta)
  float gy = 100.0;
  float gtheta = 0.0;

  // Send out startup announcement
  freqout(4, 500, 3000);                      // Speaker tone: 0.5 s @ 3 kHz, 0.25 s @ 3.5 kHz
  freqout(4, 250, 3500);

  // Set up some variables we will need
  botSetMaxSpeed(32);
  botSetRampRate(4);
 
  while(1)
  {
    // update current pose in world coordinate frame

    // calculate pose of goal in bot coordinate frame

    // calculate omega

    // calculate maximum velocity for this omega

    // set velocity and omega

    pause(100);
  } // End of while()
} // End of main()
