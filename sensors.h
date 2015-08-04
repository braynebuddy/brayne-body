//   A set of routines to get data from the ActivityBot's sensors
int pingHere();
int pingAngle(int angle);
void pingScan();

int irLeft();
int irRight();

int *startSensor();
void stopSensor();
void updateSensor();

volatile extern int pingerAngle;
volatile extern int pingFront;
volatile extern int pingLeft;
volatile extern int pingRight;

volatile extern int detectLeft;
volatile extern int detectRight;

extern int ticksL;
extern int ticksR;

volatile extern int posX;
volatile extern int posY;
volatile extern int posTheta;

extern int scanAngle[];
extern int scan_cm[];
extern int numAngles;
