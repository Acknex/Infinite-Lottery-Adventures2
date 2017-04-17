#ifndef angle_h
#define angle_h

// PUBLIC

	ANGLE* angSet (ANGLE* src, ANGLE* dst);

	ANGLE* angLerp (ANGLE* r, ANGLE* src, ANGLE* dst, float fac);
	ANGLE* angTemp (ANGLE* angle);
	void limitAngle (ANGLE* angle, float minPan, float maxPan, float minTilt, float maxTilt, float minRoll, float maxRoll);

// PRIVATE

// Code include
#include "angle.c"

#endif /* angle_h */