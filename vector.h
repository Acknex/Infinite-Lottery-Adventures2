#ifndef vector_h
#define vector_h

// PUBLIC

	// Interpolates the vector <r> between <src> and <dst> according to the factor
	// <fac>. If <r> == NULL, a temporary result vector is returned, otherwise <r>
	// is returned (pointerwise). Replaces vec_lerp, because vec_lerp also accepts
	// factors out of 0...1, which is not correct.
	VECTOR* vecLerp (VECTOR* r, VECTOR* src, VECTOR* dst, float fac);
	
	VECTOR* vecTemp (VECTOR* v);
	
	VECTOR* vecSet (VECTOR* src, VECTOR* dst);
	VECTOR* vecSet (VECTOR* dst, var x, var y, var z);

	float vecDist (VECTOR* a, VECTOR* b);

// PRIVATE

	// Overwriting engine instructions
	VECTOR* vec_lerp (VECTOR*, VECTOR*, VECTOR*, var); // Redirects to vecLerp

// Code include
#include "vector.c"

#endif /* vector_h */