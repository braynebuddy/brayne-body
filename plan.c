/*
  plan.c

   A set of routines to do planning for the ActivityBot
  ------------------------------------------------------------------------------
  Copyright 2015 Robert B. Hawkins
  Distributed under the MIT License
  (see accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
  ------------------------------------------------------------------------------

  Date        Ver   Comments
  ==========  ====  ==================================================
  2015-12-04   1.0  Initial version of gotoGoal(), avoidObstacle(), plan() 

*/
#include <math.h>                             // Needed for sin(), cos (), atan2()

#include "simpletools.h"                      // Include simpletools header
#include "plan.h"                             // Function declarations

#include "botports.h"                         // Ports in use for the ActivityBot
#include "slam.h"                             // Localization, transforms, and maps
#include "sense.h"                            // Manage sensors in use on the ActivityBot


// --- ActivityBot current plan (xdot, ydot)
float plan[] = {0.0, 0.0};

// --- Overall plan selection
void makePlan(float *goal, float *obstacle)
{
  float beta = 0.5;      // Blending factor
  float s;
  float obsD = 0.0;
  
  s = 1.0 - exp(-beta * obsD);
  
  plan[0] = 0.0;
  plan[1] = 0.0;
}
 // --- Possible planning functions
void gotoGoal(float *goal)
{
}
  
void avoidObstacle(float *obstacle)
{
}  