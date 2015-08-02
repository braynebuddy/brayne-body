/*
  sensors.c

  A set of routines to get data from the ActivityBot's sensors

  ------------------------------------------------------------------------------
  Copyright 2015 Robert B. Hawkins
  Distributed under the MIT License
  (see accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
  ------------------------------------------------------------------------------

  Date        Ver   Comments
  ==========  ====  ==================================================
  2015-06-09   1.0  Initial version for new PING))) scanner
  2015-07-17   1.1  Assume incoming argument is in bot coordinate frame
                    (i.e., 0 degrees is straight ahead)
  2015-07-18   2.0  Include everything from IR sensors and PING)))
  2015-07-31   2.1  Add cog launcher code 

*/

#include "simpletools.h"                      // Include simpletools header
#include "servo.h"                            // Include basic servo header
#include "ping.h"                             // Include ping header

#include "botports.h"                         // Ports in use for the ActivityBot
#include "sensors.h"                          // Function declarations

volatile int pingerAngle=900;
static int *cog = 0;

int *startSensor()
{
  if(!cog){
    cog = cog_run(&updateSensor, 100);
  }
}

void stopSensor()
{
  cog_end(cog);
  cog = 0;
}

void updateSensor()
{
}

int pingHere()
{
  // return distance from direction PING))) is currently pointing
  // function is used mostly to remove delay from pingAngle function

    return(ping_cm(PINGER));
}

int pingAngle(int angle)
{
  int deltaAngle;
  // Convert from bot coordinate frame to servo coordinate frame
  // (i.e., servo 0 is 90 degrees clockwise)
  angle = 10 * angle + 900;
  while (angle > 1800)
    angle = angle - 3600;
  while (angle < -1800)
    angle = angle + 3600;

  if (angle < 0) angle = 0;
  if (angle > 1800) angle = 1800; 
  servo_angle(PINGSERVO, angle+PINGBIAS);

  // Allow time for sensor to move
  deltaAngle = pingerAngle > angle ? pingerAngle-angle : angle-pingerAngle;
//  print("pingerAngle = %d, angle = %d, deltaAngle = %d %c\n", pingerAngle, angle, deltaAngle, CLREOL);
  if(deltaAngle > 10) pause(deltaAngle/2);

  pingerAngle = angle;

  return(ping_cm(PINGER));
}

int irLeft()
{
  int ir;
  low(DA_ZERO);                           // D/A 0 to 0 V
  freqout(LEFT_IR_LED, 1, 38000);         // Left IR LED light
  ir = input(LEFT_IR_DET)==0 ? 1: 0;      // IR detector - 0 means found
  if (ir) print("irLeft detected.");
  return(ir);
}

int irRight()
{
  int ir;
  low(DA_ZERO);                           // D/A 1 to 0 V
  freqout(RIGHT_IR_LED, 1, 38000);        // Right IR detector light
  ir = input(RIGHT_IR_DET)==0 ? 1 : 0;    // IR detector - 0 means found
  if (ir) print("irRight detected.");
  return(ir);
}

