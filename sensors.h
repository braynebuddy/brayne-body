//   A set of routines to get data from the ActivityBot's sensors
int pingHere();
int pingAngle(int angle);
int irLeft();
int irRight();

int *startSensor();
void stopSensor();
void updateSensor();

volatile extern int pingerAngle;
