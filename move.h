// Routines that cause the ActivityBot to move in a variety of ways.
#ifndef _MOVE_H_
#define _MOVE_H_

// Some constants
#define round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))
#define M_PI  3.141592654
#define M_2PI (2.0*M_PI)

// Wheel speeds in ticks/sec
extern int botSpeed;
extern int leftSpeed;        // ticks/s
extern int rightSpeed;       // ticks/s

//   A set of routines to make the ActivityBot move
int turnTicks(int a);

void botRotation(float omega);
void botTurnAngle(int angle);
void botTurnHeading(int angle, int duration);

void botMove(int distance_cm);
void botTurn(float a);

void botSetMaxSpeed(int s);
void botSetRampRate(int r);
void botSetDeltaSpeed(int d);
void botStop();

void botSetSpeed(float mmps);
void botSetVW(float velocity, float omega);
float pid_omega(float xy[2]);

#endif
