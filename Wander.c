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

  pingAngle(0);
  // Find the best direction to go
  //while (1) { // for testing  
  //pause(1000);
  print("%c", HOME);
  for (int i = 0; i < aLen ; i++) {
    d = pingAngle(a[i]);
    print("angle = %d, distance = %d %c\n", a[i], d, CLREOL);
    if (d > dMax) {
      dMax = d;
      aMax = a[i];
    }
  }
  //} // end of while(1) for testing 
  botTurnAngle(aMax);                         // Turn in best direction
  botSetSpeed(normalSpeed);                   // Start Bot moving at desired speed
  
  while(1)
  {
    //if (irLeft()  && !irRight()) botTurnHeading(-200, 1);
    //if (irRight() && !irLeft())  botTurnHeading( 200, 1);

    // Check for obstacles
    dFront = pingAngle(0);
    if (dFront < minApproach) {
      // Obstacle ahead, so stop and find a good direction
      freqout(4, 100, 3000);
      botSetSpeed(0);
      botMove(-5 * minApproach); // move back minApproach/2 cm
      dMax = 0;
      while (dMax < 2 * minApproach)
      {
        for (int i = 0; i < aLen ; i++) {
          d = pingAngle(a[i]);
          print("angle = %d, distance = %d %c\n", a[i], d, CLREOL);
          if (d > dMax) {
            dMax = d;
            aMax = a[i];
          }
        }        
        if (dMax < 2 * minApproach) {
          freqout(4, 100, 3000);
          freqout(4, 100, 3000);
          //botMove(-50);
          botTurnAngle(180);
        }
      }
      // Turn in best direction
      botTurnAngle(aMax);
      // Get a new distance, and go!
      dFront = pingAngle(0);
      botSetSpeed(normalSpeed);                              
    }

    if (dFront < minApproach / 2 ){
      // Emergency!
      freqout(4, 100, 3000);
      freqout(4, 100, 3000);
      freqout(4, 100, 3000);
      // Stop, backup 15 cm, and turn around.
      botSetSpeed(0);
      botMove(-30 * minApproach);
      botTurnAngle(180);
      //  Look for a better path.
      dMax = 0;        
      for (int i = 0; i < aLen ; i++) {
        d = pingAngle(a[i]);
        if (d > dMax) {
          dMax = d;
          aMax = a[i];
        }
      }        
      // Turn in best direction and go
      botTurnAngle(aMax);
      botSetSpeed(normalSpeed);
    }        
    pause(500);
  }                             // end of while(1) loop  
}                               // end of main()
