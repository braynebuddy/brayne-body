/*
  slam.c

   A set of routines to do simultaneous localization and mapping (SLAM)
   for the ActivityBot
  ------------------------------------------------------------------------------
  Copyright 2015 Robert B. Hawkins
  Distributed under the MIT License
  (see accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
  ------------------------------------------------------------------------------

  Date        Ver   Comments
  ==========  ====  ==================================================
  2015-12-01   1.0  Initial version of updatePose() 

*/
#include <math.h>                             // Needed for sin(), cos (), atan2()

#include "simpletools.h"                      // Include simpletools header

#include "botports.h"                         // Ports in use for the ActivityBot
#include "slam.h"                             // Function declarations

// --- ActivityBot pose (x,y,theta)
float botP[2];
float botTheta;

/*
void updatePose()
{
  int newL, newR;
  int deltaL, deltaR;
  drive_getTicks(&newL, &newR);
  print ("ticksL = %d ticksR = %d%c\n", ticksL, ticksR, CLREOL);
  print ("newL = %d newR = %d%c\n", newL, newR, CLREOL);
  deltaL = newL - ticksL;
  deltaR = newR - ticksR;
  print ("deltaL = %d deltaR = %d%c\n", deltaL, deltaR, CLREOL);

  botP[0] += (3.25 * (deltaL + deltaR) / 2.0) * cos(botTheta);
  botP[1] += (3.25 * (deltaL + deltaR) / 2.0) * sin(botTheta);;
  botTheta += (33.1/105.8) * (deltaR - deltaL);

  ticksL = newL;
  ticksR = newR;
}
*/
void updatePose()
{
  int newL = 0;
  int newR = 0;
  int deltaL, deltaR;

  float R, omega_dt;
  float ICC[2];
  float L = 105.8;
  float step = 3.25;
  float X, Y;

  drive_getTicks(&newL, &newR);
  deltaL = newL - ticksL;
  deltaR = newR - ticksR;

  if (deltaL == deltaR) {
    X = botP[0] + deltaL * step * cos(botTheta);
    Y = botP[1] + deltaL * step * sin(botTheta);
    omega_dt = 0.0;

  } else {
    R = (L/2.0) * (deltaR + deltaL) / (deltaR - deltaL);
    omega_dt = (deltaR - deltaL) * step / L;
    ICC[0] = botP[0] - R * sin(botTheta);
    ICC[1] = botP[1] + R * cos(botTheta);
    X = ICC[0] + (botP[0]-ICC[0])*cos(omega_dt) - (botP[1]-ICC[1])*sin(omega_dt);
    Y = ICC[1] + (botP[0]-ICC[0])*sin(omega_dt) + (botP[1]-ICC[1])*cos(omega_dt);
  }

  botP[0] = X;
  botP[1] = Y;
  botTheta += omega_dt;

  ticksL = newL;
  ticksR = newR;
  print ("updatePose: ticksL = %d ticksR = %d%c\n", ticksL, ticksR, CLREOL);
  print ("updatePose: deltaL = %d deltaR = %d%c\n", deltaL, deltaR, CLREOL);
  print ("updatePose: X = %f Y = %f theta = %f%c\n", botP[0], botP[1], botTheta, CLREOL);
}
