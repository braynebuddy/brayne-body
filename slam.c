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
  2015-12-02   1.1  Merge in coordinate transform functions

*/
#include <math.h>                             // Needed for sin(), cos (), atan2()

#include "simpletools.h"                      // Include simpletools header

#include "botports.h"                         // Ports in use for the ActivityBot
#include "slam.h"                             // Function declarations
#include "sense.h"                            // Manage sensors in use on the ActivityBot

// --- ActivityBot current pose (x,y,theta)
float botP[3];

// --- Coordinate Transforms
void aTb(float *aP, float *bP, float *b)
{
  // In:
  // bP[] is (x,y) of the point wrt coord frame b
  // b[] is (x,y,theta) of B wrt A 
  // Out:
  // aP[] is (x,y) of the point wrt coord frame A

  aP[0] = bP[0]*cos(b[2]) - bP[1]*sin(b[2]) + b[0];
  aP[1] = bP[0]*sin(b[2]) + bP[1]*cos(b[2]) + b[1];
}

void aTb_inv(float *aP, float *bP, float *b)
{
  // In:
  // aP[] is (x,y) of the point wrt coord frame A
  // b[] is (x,y,theta) of B wrt A 
  // Out:
  // bP[] is (x,y) of the point wrt coord frame b
  bP[0] = (aP[0]-b[0])*cos(b[2]) + (aP[1]-b[1])*sin(b[2]);
  bP[1] = -(aP[0]-b[0])*sin(b[2]) + (aP[1]-b[1])*cos(b[2]);
}

// --- Localization
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
    X = botP[0] + deltaL * step * cos(botP[2]);
    Y = botP[1] + deltaL * step * sin(botP[2]);
    omega_dt = 0.0;

  } else {
    R = (L/2.0) * (deltaR + deltaL) / (deltaR - deltaL);
    omega_dt = (deltaR - deltaL) * step / L;
    ICC[0] = botP[0] - R * sin(botP[2]);
    ICC[1] = botP[1] + R * cos(botP[2]);
    X = ICC[0] + (botP[0]-ICC[0])*cos(omega_dt) - (botP[1]-ICC[1])*sin(omega_dt);
    Y = ICC[1] + (botP[0]-ICC[0])*sin(omega_dt) + (botP[1]-ICC[1])*cos(omega_dt);
  }

  botP[0] = X;
  botP[1] = Y;
  botP[2] += omega_dt;

  ticksL = newL;
  ticksR = newR;
  //print ("updatePose: ticksL = %d ticksR = %d%c\n", ticksL, ticksR, CLREOL);
  //print ("updatePose: deltaL = %d deltaR = %d%c\n", deltaL, deltaR, CLREOL);
  //print ("updatePose: X = %f Y = %f theta = %f%c\n", botP[0], botP[1], botTheta, CLREOL);
}
