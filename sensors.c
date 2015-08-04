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
#include "abdrive.h"                          // Include abdrive header

#include "botports.h"                         // Ports in use for the ActivityBot
#include "sensors.h"                          // Function declarations

static int *cog = 0;

volatile int pingerAngle = 900;
volatile int pingFront = 1000;
volatile int pingLeft = 1000;
volatile int pingRight = 1000;

volatile int detectLeft = 0;
volatile int detectRight = 0;

int ticksL = 0;
int ticksR = 0;

volatile int posX = 0;
volatile int posY = 0;
volatile int posTheta = 0;

int scanAngle[] = {-90, -80, -70, -60, -50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
int scan_cm[]   = {  0,   0,   0,   0,   0,   0,   0,   0,   0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
int numAngles   = sizeof(scanAngle) / sizeof(*scanAngle);

int *startSensor()
{
  if(!cog){
    cog = cog_run(&updateSensor, 100);
  }
  return cog;
}

void stopSensor()
{
  cog_end(cog);
  cog = 0;
}

void updateSensor()
{
  int baseL, baseR;
  drive_getTicks(&baseL, &baseR);
  ticksL = 0;
  ticksR = 0;
  pingLeft = pingAngle(90);
  pingRight = pingAngle(-90);
  pingFront = pingAngle(0);

  //while (1)
  //{
    detectLeft = irLeft();
    if (detectLeft) {
      pingLeft = pingAngle(90);
    }

    detectRight = irRight();
    if (detectRight) {
      pingRight = pingAngle(-90);
    }

    pingFront = pingAngle(0);

    drive_getTicks(&ticksL, &ticksR);
    ticksL = ticksL - baseL;
    ticksR = ticksR - baseR;

  //}
}

int pingHere()
{
  // return distance from direction PING))) is currently pointing
  // function is used mostly to remove delay from pingAngle function

    return(ping_cm(PINGER));
}

void pingScan()
{
  for (int i=0; i < numAngles; i++) {
    if (scanAngle[i] < -90) scanAngle[i] = -90;
    if (scanAngle[i] >  90) scanAngle[i] =  90;
    scan_cm[i] = pingAngle(scanAngle[i]);
  }
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
  //if (ir) print("irLeft detected.");
  return(ir);
}

int irRight()
{
  int ir;
  low(DA_ZERO);                           // D/A 1 to 0 V
  freqout(RIGHT_IR_LED, 1, 38000);        // Right IR detector light
  ir = input(RIGHT_IR_DET)==0 ? 1 : 0;    // IR detector - 0 means found
  //if (ir) print("irRight detected.");
  return(ir);
}

