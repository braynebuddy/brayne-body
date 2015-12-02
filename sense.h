//   A set of routines to get data from the ActivityBot's sensors

// --- PING))) scanner
extern int pingerAngle;
extern int pingFront;
extern int pingLeft;
extern int pingRight;
extern int scanAngle[];
extern int scan_cm[];
extern int numAngles;

int pingHere();
int pingAngle(int angle);
void pingScan();

// --- IR Sensors
extern int detectLeft;
extern int detectRight;

int irLeft();
int irRight();

// --- Odometry
extern int ticksL;
extern int ticksR;

void updateTicks();

// --- General scanning functions
int *startSensor();
void stopSensor();
void updateSensor();
