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

void aTb(float aP[2], float bP[2], float x, float y, float theta)
{
  // aP[] is the point wrt coord frame A --->(out)
  // bP[] is the point wrt coord frame b <---(in)
  // (x,y) is the translation of B origin wrt A
  // theta is the rotation of B wrt A 
  aP[0] = bP[0]*cos(theta) - bP[1]*sin(theta) + x;
  aP[1] = bP[0]*sin(theta) + bP[1]*cos(theta) + y;
}

void aTb_inv(float aP[2], float bP[2], float x, float y, float theta)
{
  // aP[] is the point wrt coord frame A <---(in)
  // bP[] is the point wrt coord frame b --->(out)
  // (x,y) is the translation of B origin wrt A
  // theta is the rotation of B wrt A 
  bP[0] = (aP[0]-x)*cos(theta) + (aP[1]-y)*sin(theta);
  bP[1] = -(aP[0]-x)*sin(theta) + (aP[1]-y)*cos(theta);
}