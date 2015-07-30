/*
  pingAngle.c

  Use the servo to scan from angle[0] to angle[9] and save distances for each angle

  ------------------------------------------------------------------------------
  Copyright 2015 Robert B. Hawkins
  Distributed inder the MIT License
  (see accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
  ------------------------------------------------------------------------------

  Date        Ver   Comments
  ==========  ====  ==================================================
  2015-06-09   1.0  Initial version for new PING))) scanner
  2015-07-17   1.1  Assume incoming argument is in bot coordinate frame
                    (i.e., 0 degrees is straight ahead)

*/

#include "simpletools.h"                      // Include simpletools header
#include "servo.h"                            // Include basic servo header
#include "ping.h"                             // Include ping header

#include "botports.h"                         // Ports in use for the ActivityBot

int pingAngle(int angle)
{
  // Convert from bot coordinate frame to servo coordinate frame
  // (i.e., servo 0 is 90 degrees clockwise)
  angle = angle + 900;
  while (angle > 1800)
    angle = angle - 3600;
  while (angle < -1800)
    angle = angle + 3600;

  if (angle < 0) angle = 0;
  if (angle > 1800) angle = 1800; 
  //servo_setramp(PINGSERVO, 50);
  servo_angle(PINGSERVO, angle+PINGBIAS);
  pause(600);
  return(ping_cm(PINGER));
}

