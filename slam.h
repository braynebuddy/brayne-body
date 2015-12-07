//   A set of routines to do simultaneous localization and mapping (SLAM)
//   for the ActivityBot
#ifndef _SLAM_H_
#define _SLAM_H_

// --- Coordinate transforms
void aTb(float *aP, float *bP, float *b);
void aTb_inv(float *aP, float *bP, float *b);

// --- Localization
extern float botP[3];

void updatePose();

#endif
