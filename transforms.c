/*
  transforms.c

  A set of routines to calculate coordinate frame transformations

  ------------------------------------------------------------------------------
  Copyright 2015 Robert B. Hawkins
  Distributed under the MIT License
  (see accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
  ------------------------------------------------------------------------------

  Date        Ver   Comments
  ==========  ====  ==================================================
  2015-08-18   1.0  Initial version

*/

#include <math.h>                             // Needed for sin(), cos (), atan2()

#include "botports.h"                         // Ports in use for the ActivityBot
#include "transforms.h"                       // Coordinate frame transforms

void aTb(float aP[2], float bP[2], float b[3])
{
  // In:
  // bP[] is (x,y) of the point wrt coord frame b
  // b[] is (x,y,theta) of B wrt A 
  // Out:
  // aP[] is (x,y) of the point wrt coord frame A

  aP[0] = bP[0]*cos(b[2]) - bP[1]*sin(theta) + b[0];
  aP[1] = bP[0]*sin(b[2]) + bP[1]*cos(theta) + b[1];
}

void aTb_inv(float aP[2], float bP[2], float b[3])
{
  // In:
  // aP[] is (x,y) of the point wrt coord frame A
  // b[] is (x,y,theta) of B wrt A 
  // Out:
  // bP[] is (x,y) of the point wrt coord frame b
  bP[0] = (aP[0]-b[0])*cos(b[2]) + (aP[1]-b[1])*sin(b[2]);
  bP[1] = -(aP[0]-b[0])*sin(b[2]) + (aP[1]-b[1])*cos(b[2]);
}
