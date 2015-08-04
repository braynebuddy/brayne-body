/*
  Tester.c
  --------
  A simple test harness for new low-level robot modules.
  Based on examples from http://learn.parallax.com/activitybot
  and http://learn.parallax.com/propeller-c-tutorials 

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
  // Send out startup announcement
  freqout(4, 500, 3000);                      // Speaker tone: 0.5 s @ 3 kHz, 0.25 s @ 3.5 kHz
  freqout(4, 250, 3500);
 
  while(1)
  {
    // Add main loop code here.
    print("%c", HOME);

    updateSensor();
    print("detectLeft  = %d %c\n", detectLeft, CLREOL);
    print("pingLeft  = %d %c\n", pingLeft, CLREOL);

    print("detectRight = %d %c\n", detectRight, CLREOL);
    print("pingRight = %d %c\n", pingRight, CLREOL);

    pingScan();
    for (int i=0; i < numAngles; i++) {
      print("scanAngle = %d, scan_cm = %d %c\n", scanAngle[i], scan_cm[i], CLREOL);
    }
    pause(1000);
    
  }  // end of while (1)
} // End of main()
