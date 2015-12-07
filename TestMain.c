/*
  TestMain.c
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
#include "sense.h"                            // Manage sensors in use on the ActivityBot
#include "move.h"                             // Move the ActivityBot around
#include "slam.h"                             // Localization, transforms, and Mapping

int main()                                    // Main function
{
  // Send out startup announcement
  freqout(4, 500, 3000);                      // Speaker tone: 0.5 s @ 3 kHz, 0.25 s @ 3.5 kHz
  freqout(4, 250, 3500);

  float goal[] = {400.0, 200.0, 0.0};
  float plan[2];
   
  while(1)
  {
    // Add main loop code here.
    print("%c", HOME);

    updateSensor();
    //pingScan();
    updatePose();
    
    print("detectLeft  = %d %c\n", detectLeft, CLREOL);
    print("pingLeft  = %d %c\n", pingLeft, CLREOL);
    print("detectRight = %d %c\n", detectRight, CLREOL);
    print("pingRight = %d %c\n", pingRight, CLREOL);
    for (int i=0; i < numAngles; i++) {
      print("scanAngle = %d, scan_cm = %d %c\n", scanAngle[i], scan_cm[i], CLREOL);
    }

    makePlan(plan);
    
    executePlan(plan);

    pause(100);
    
  }  // end of while (1)
} // End of main()