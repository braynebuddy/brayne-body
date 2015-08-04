/*
  Wander.c

  Detect obstacles in the ActivityBot's path, and turn to avoid them.
  Based on AvoidObstacles.c from http://learn.parallax.com/activitybot

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

int main()                                    // Main function
{
  int normalSpeed = 32;                       // ticks/second -- 64 ticks = 1 revolution, max is 128
  int normalRampRate = 2;                     // ticks/sec per 20ms (default is 4)
  int loopPause = 500;                        // pause at end of scan loop
  int minApproach = 2 * ((normalSpeed * 13 / 4) * loopPause / 1000) / 10;
  int dFront;
  int d;
  int dMax;
  int aMax;
  //int a[] = {-90, -80, -70, -60, -50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
  int a[] = {-90, 90, 0};
  int aLen = sizeof(a) / sizeof(*a);

  // Send out startup announcement
  freqout(4, 500, 3000);                      // Speaker tone: 0.5 s @ 3 kHz, 0.25 s @ 3.5 kHz
  freqout(4, 250, 3500);

  // Set up some variables we will need
  botSetMaxSpeed(normalSpeed);
  botSetRampRate(normalRampRate);

  startSensor();                              // Start scanning

  //while (1) { // for testing  
  //pause(1000);
  //print("%c", HOME);

  // Find the best direction to go
  if (pingFront < 5 * minApproach) {
    if (!detectLeft) {
      botTurnAngle(90);
    }
    else if (!detectRight) {
      botTurnAngle(-90);
    }
    else {
      botTurnAngle(180);
    }
  }
  //} // end of while(1) for testing 
  botSetSpeed(normalSpeed);                   // Start Bot moving at desired speed
  
  while(1)
  {
    // First make sure we're not about to run into something
    if (pingFront < minApproach) {
      // Obstacle ahead
      freqout(4, 100, 3500);
      freqout(4, 100, 3500);
      if (!detectLeft) {
        botTurnAngle(90);
      } else if (!detectRight) {
        botTurnAngle(-90);
      } else {
        botTurnAngle(180);
      }
      botSetSpeed(normalSpeed);
    }
/*
    // Now see if we need to track a wall on the left or right.
    if (detectLeft & detectRight) {
      // In a narrow hallway, so try to stay in the middle
      freqout(4, 100, 2500);
      freqout(4, 100, 2500);
      if ((pingLeft-pingRight) > 1) {
        // Turn left by setting speed delta positive
        botSetDeltaSpeed(4);
      } else if ((pingLeft-pingRight) < -1){
        // Turn right by setting speed delta negative
        botSetDeltaSpeed(-4);
      } else {
        // No turning
        botSetDeltaSpeed(0);
      }
    } 
    else {
      if (detectLeft) {
        freqout(4, 100, 2500);
        freqout(4, 100, 3500);
        // Obstacle on left
        if (pingLeft < 10) {
          // Turn right by setting speed delta negative
          botSetDeltaSpeed(-4);
        }
      }
      if (detectRight) {
        // Obstacle on right
        freqout(4, 100, 3500);
        freqout(4, 100, 2500);
        if (pingRight < 10) {
          // Turn left by setting speed delta positive
          botSetDeltaSpeed(4);
        }
      }
    }
    freqout(4, 100, 2000);
    pause(500);
*/
  }                             // end of while(1) loop  
}                               // end of main()
