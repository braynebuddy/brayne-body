//   A set of routines to make the ActivityBot move
int turnTicks(int a);

void botRotation(float omega);
void botTurnAngle(int angle);
void botTurnHeading(int angle, int duration);

void botMove(int distance_cm);

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


