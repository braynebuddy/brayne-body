//   A set of routines to do simultaneous localization and mapping (SLAM)
//   for the ActivityBot
#ifndef _SLAM_H_
#define _SLAM_H_

// --- Coordinate transforms
void aTb(float aP[2], float bP[2], float b[3]);
void aTb_inv(float aP[2], float bP[2], float b[3]);

// --- Localization
extern float botP[3];

void updatePose();

#endif
