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
  // aP[] is the point wrt coord frame A
  // bP[] is the point wrt coord frame b
  // (x,y) is the translation from A to B
  // theta is the rotation of B wrt A 
  bP[0] = aP[0]*cos(theta) - aP[1]*sin(theta) + x;
  bP[1] = aP[0]*sin(theta) + aP[1]*cos(theta) + y;
}