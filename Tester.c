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
  int running;
  running = 1;
  // Send out startup announcement
  freqout(4, 500, 3000);                      // Speaker tone: 0.5 s @ 3 kHz, 0.25 s @ 3.5 kHz
  freqout(4, 250, 3500);
 

  while(running)
  {
    // Add main loop code here.
    print("%c", HOME);
    updateSensor();
    print("Moving %d cm%c\n", pingFront - 5, CLREOL);
    botMove(10*pingFront - 100);
    //botMove(0);
    
    updateSensor();
    print("detectLeft  = %d %c\n", detectLeft, CLREOL);
    print("pingLeft  = %d %c\n", pingLeft, CLREOL);

    print("detectRight = %d %c\n", detectRight, CLREOL);
    print("pingRight = %d %c\n", pingRight, CLREOL);

    print("pingFront = %d %c\n", pingFront, CLREOL);
    
    if (pingLeft < 20 && pingRight >= 20) {
        botTurn(-M_PI/2.0);
    } else if (pingRight < 20 && pingLeft >= 20) {
        botTurn(M_PI/2.0);
    } else if (pingLeft < 20 && pingRight < 20) {
        botTurn(M_PI);
    } else {
          if (pingLeft < pingRight) {
            botTurn(M_PI/2.0);
          } else {
            botTurn(-M_PI/2.0);
          }                                            
    }                   
    if (pingLeft < 10 && pingRight < 10) {
      running = 0;
    }      
    pause(1000);
    
  }  // end of while (1)
} // End of main()
