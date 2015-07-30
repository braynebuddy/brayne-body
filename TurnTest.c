/*
  Simple test harness for turning trials

  ------------------------------------------------------------------------------
  Copyright 2015 Robert B. Hawkins
  Distributed inder the MIT License
  (see accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
  ------------------------------------------------------------------------------

*/
#include <stdlib.h>                           // for rand()
#include "simpletools.h"                      // Include simpletools header

#include "botports.h"                         // Ports in use for the ActivityBot

//volatile int angle[] = {1400, 500, 950}; 
//volatile int angle[] = {750,800,850,900,950,1000,1050,1100, 1150}; 

int pingAngle();
void turnAngle();


int main()                                    // Main function
{
  int angle;
  int distance;
  
  freqout(4, 500, 3000);                      // Speaker tone: 0.5 s @ 3 kHz, 0.25 s @ 3.5 kHz
  freqout(4, 250, 3500);

 
  while(1)
  {
    print("%cEnter angle: %c",HOME,CLREOL);
    scan("%d\n", &angle);
    //angle = 10*((rand() % 180));
    distance = pingAngle(10*angle);
    print("Angle = %d, distance = %d %c\n", angle, distance, CLREOL);
    turnAngle(10*angle);
    pingAngle(0);    
  }  
}

