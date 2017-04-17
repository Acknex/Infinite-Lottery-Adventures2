#ifndef angle_c
#define angle_c

#include "angle.h"

ANGLE* angSet (ANGLE* src, ANGLE* dst)
{
	if ((src == NULL) || (dst == NULL)) return;

	dst->pan = src->pan;
	dst->tilt = src->tilt;
	dst->roll = src->roll;

	return(dst);
}

ANGLE* angLerp (ANGLE* _r, ANGLE* src, ANGLE* dst, float _fac)
{
	if ((src == NULL) || (dst == NULL))
		return(NULL);

	float fac = maxv(0.0, minv(_fac, 1.0)); // Clamp factor between 0..1

	VECTOR angularDiff;
	VECTOR r;

	// todo: ang_diff?
	vec_diff(&angularDiff, dst, src);

	angularDiff.x = ang(angularDiff.x); // Adjust angle validity
	angularDiff.y = ang(angularDiff.y);
	angularDiff.z = ang(angularDiff.z);

	vec_scale(&angularDiff, (var)fac); // Interpolation

   // Set interpolated angle
	vec_set(r, src);
	vec_add(r, &angularDiff);
	
	if (_r == NULL)
		return((ANGLE*)vector(r.x, r.y, r.z));
	else
	{
		vec_set(_r, r);
		return(_r);
	}
}

ANGLE* angTemp (ANGLE* angle)
{
	return((ANGLE*)vector(angle.pan, angle.tilt, angle.roll));
}

void limitAngle (ANGLE* angle, float minPan, float maxPan, float minTilt, float maxTilt, float minRoll, float maxRoll)
{
	angle->pan  = maxv(minPan,  minv(angle->pan,  maxPan));
	angle->tilt = maxv(minTilt, minv(angle->tilt, maxTilt));
	angle->roll = maxv(minRoll, minv(angle->roll, maxRoll));
}

#endif /* angle_c */