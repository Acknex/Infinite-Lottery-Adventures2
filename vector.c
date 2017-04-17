#ifndef vector_c
#define vector_c

#include "vector.h"

VECTOR* vecLerp (VECTOR* _r, VECTOR* src, VECTOR* dst, float _fac)
{
	if ((src == NULL) || (dst == 0))
		return NULL;

	float fac = maxv(0.0, minv(_fac, 1.0)); // Clamp fac to 0...1

	if (_r != NULL)
	{
		_r->x = src->x + (dst->x - src->x) * fac;
		_r->y = src->y + (dst->y - src->y) * fac;
		_r->z = src->z + (dst->z - src->z) * fac;
		return(_r);
	}
	else
	{
		return vector(src->x + (dst->x - src->x) * fac,
		              src->y + (dst->y - src->y) * fac,
				        src->z + (dst->z - src->z) * fac);
	}
}

VECTOR* vec_lerp (VECTOR* r, VECTOR* src, VECTOR* dest, var fac)
{
	return(vecLerp(r, src, dest, fac));
}

VECTOR* vecTemp (VECTOR* v)
{
	if (v == NULL)
		return(NULL);
		
	return(vector(v.x, v.y, v.z));
}

VECTOR* vecSet (VECTOR* dst, var x, var y, var z)
{
	VECTOR* v = dst;

	if (v == NULL)
	{
		v = vector(x,y,z);
	}
	else
	{
		v->x = x;
		v->y = y;
		v->z = z;
	}

	return(v);
}

VECTOR* vecSet (VECTOR* src, VECTOR* dst)
{
	if ((src == NULL) || (dst == NULL))
	{
		return(NULL);
	}
	
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
	
	return(dst);
}

float vecDist (VECTOR* a, VECTOR* b)
{
	float xDiff = (float)a.x - (float)b.x;
	float yDiff = (float)a.y - (float)b.y;
	float zDiff = (float)a.z - (float)b.z;

	return(sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff));
}

#endif /* vector_c */