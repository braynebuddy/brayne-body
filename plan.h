//   A set of routines to do planning for the ActivityBot
#ifndef _PLAN_H_
#define _PLAN_H_

// --- Planning
extern float plan[2];

void makePlan(float *goal, float *obstacle);
void gotoGoal(float *goal);
void avoidObstacle(float *obstacle);

#endif
