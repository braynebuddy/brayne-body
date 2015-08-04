//   A set of routines to make the ActivityBot move
int turnTicks(int a);

void botRotation(int omega);
void botTurnAngle(int angle);
void botTurnHeading(int angle, int duration);

void botMove(int distance_cm);

void botSetMaxSpeed(int s);
void botSetRampRate(int r);
void botSetSpeed(int s);
void botSetDeltaSpeed(int d);

volatile extern int botSpeed;


