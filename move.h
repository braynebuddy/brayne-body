// Some constants
#define round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

// PI = 3.1415926535897932384626433832795 (probably defined in <math.h>)
#ifndef M_PI
#define M_PI  3.141592654
#endif 



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

volatile extern int botSpeed;
volatile extern int leftSpeed;        // ticks/s
volatile extern int rightSpeed;       // ticks/s


