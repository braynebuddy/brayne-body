/*
  turnAngle.c

  Stop the ActivityBot and turn the requested number of degrees.

  ------------------------------------------------------------------------------
  Copyright 2015 Robert B. Hawkins
  Distributed inder the MIT License
  (see accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
  ------------------------------------------------------------------------------

  Date        Ver   Comments
  ==========  ====  ==================================================
  2015-07-17   1.0  Separate turnAngle() into it's own file, correct 
                    coordinate frames to us bot reference.

*/
#include "simpletools.h"                      // Include simpletools header
#include "abdrive.h"                          // Include abdrive header

void turnAngle(int a)
{
  // Assume incoming angle is in bot coordinate frame (i.e. 0 degrees is
  // directly ahead, angles increase counter-clockwise).
  // For the ActivityBot, Turning radius is 105.8 mm, so turning circle
  // in 664.761 mm. Encoder ticks are 3.25 ticks/mm, so 204.542 ticks will
  // produce 360 degrees of turning.
  //
  // Assume we want half of the turn on each wheel.

  int l_ticks = 0;
  int r_ticks = 0;
  int turn_ticks = 0;

  drive_ramp(0,0);
  // adjust angle to be -180 to + 180 range
  while (a > 1800)
    a = a - 3600;
  while (a < -1800)
    a = a + 3600;

  //turn_ticks = 409 * a / 7200; // ~ 204.54 / 3600.00. Has negative bias of ~2 degrees.
  //print("Turn angle = %d %c", a/10, CLREOL); 
  turn_ticks = (10*a + 88) / 176; // 1 tick = 360/204.542 = 1.76 degrees. This formula has < 1 deg error.
  //print("Turn ticks = %d %c", turn_ticks, CLREOL); 

  // Angles increase counter-clockwise, so positive angle = positive turn ricks = positive right-wheel ticks
  r_ticks = turn_ticks / 2;
  l_ticks = r_ticks - turn_ticks;

  drive_goto(l_ticks, r_ticks);
  drive_ramp(0,0);
}

