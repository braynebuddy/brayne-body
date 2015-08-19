//   A set of routines to get data from the ActivityBot's sensors
int pingHere();
int pingAngle(int angle);
void pingScan();

int irLeft();
int irRight();

int *startSensor();
void stopSensor();
void updateSensor();
void updatePose();

volatile extern int pingerAngle;
volatile extern int pingFront;
volatile extern int pingLeft;
volatile extern int pingRight;

volatile extern int detectLeft;
volatile extern int detectRight;

volatile extern int ticksL;
volatile extern int ticksR;

volatile extern float botP[2];
volatile extern float botTheta;

extern int scanAngle[];
extern int scan_cm[];
extern int numAngles;
