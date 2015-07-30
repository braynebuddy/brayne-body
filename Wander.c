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
  int normalSpeed = 64;
  int dFront;
  int d;
  int dMax;
  int aMax;
  int a[] = {-900, -450, 450, 900, 0};
  int aLen = sizeof(a) / sizeof(*a);

  // Send out startup announcement
  freqout(4, 500, 3000);                      // Speaker tone: 0.5 s @ 3 kHz, 0.25 s @ 3.5 kHz
  freqout(4, 250, 3500);

  // Set up some variables we will need
  print("%c", HOME);
  botSetMaxSpeed(64);                          // ticks/second -- 64 ticks = 1 revolution, max is 128
  botSetRampRate(12);                          // ticks/sec per 20ms (default is 4)

  pingAngle(0);
  botSetSpeed(normalSpeed);                         // Start Bot moving at desired speed
  
  while(1)
  {
    if (irLeft()  && !irRight()) botTurnHeading(-200, 1);
    if (irRight() && !irLeft())  botTurnHeading( 200, 1);

    // Check for obstacles
    dFront = pingAngle(0);
    if (dFront < 30) {
      // Obstacle ahead, so slow down and find a good direction
      botSetSpeed(normalSpeed/4);
      dMax = 0;        
      for (int i = 0; i < aLen ; i++) {
        d = pingAngle(a[i]);
        print("angle = %d, distance = %d %c\n", a[i], d, CLREOL);
        if (d > dMax) {
          dMax = d;
          aMax = i;
        }
      }        
      // Turn in best direction
      botTurnHeading(a[aMax], 1);
      botSetSpeed(normalSpeed);                              
    }

    if (dFront < 10){
      // Emergency! Stop and turn around.
      botTurnAngle(1800);
      //  Look for a better path.
      dMax = 0;        
      for (int i = 0; i < aLen ; i++) {
        d = pingAngle(a[i]);
        if (d > dMax) {
          dMax = d;
          aMax = i;
        }
      }        
      // Turn in best direction and go
      botTurnAngle(a[aMax]);
      botSetSpeed(normalSpeed);
    }        

    // Add main loop code here.
    //print("main: turning 90 degr");
    //botTurnHeading(900, 5);
    //pause(3000);
  }                             // end of while(1) loop  
}                               // end of main()
