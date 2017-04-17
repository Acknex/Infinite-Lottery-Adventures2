#ifndef target_h
#define target_h

#include "angle.h"
#include "vector.h"

// PUBLIC

	typedef struct TargetValue
	{
		float current;
		float destination;
		BOOL linear;
		float blendFactor;
	} TargetValue;

	TargetValue* newTargetValue (BOOL linear);
	void deleteTargetValue (TargetValue** targetValue);
	void setTargetValue (TargetValue** targetValue, float current, float destination);
	void setTargetValueCurrent (TargetValue** targetValue, float current);
	void setTargetValueTarget (TargetValue** targetValue, float destination);
	void setTargetValueBlendFactor (TargetValue** targetValue, float factor);
	void moveTargetValue (TargetValue** targetValue, float factor);
	float getTargetValueCurrent (TargetValue** targetValue);
	float getTargetValueDestination (TargetValue** targetValue);
	void storeTargetValueInSkill (TargetValue** targetValue, ENTITY* ent, int skillID);
	TargetValue* retrieveTargetValueFromSkill (ENTITY* ent, int skillID);

	typedef struct TargetVector
	{
		VECTOR current;
		VECTOR destination;
		BOOL linear;
		float blendFactor;
	} TargetVector;

	TargetVector* newTargetVector (BOOL linear);
	void initTargetVector (TargetVector** t, VECTOR* vCurr, VECTOR* vTarget, float blend);
	void deleteTargetVector (TargetVector** targetVector);
	void setTargetVector (TargetVector** targetVector, VECTOR* current, VECTOR* destination);
	void setTargetVectorCurrent (TargetVector** targetVector, VECTOR* current);
	void setTargetVectorTarget (TargetVector** targetVector, VECTOR* destination);
	void setTargetVectorBlendFactor (TargetVector** targetVector, float factor);
	void moveTargetVector (TargetVector** targetVector, float factor);
	VECTOR* getTargetVectorCurrent (TargetVector** targetVector);
	VECTOR* getTargetVectorDestination (TargetVector** targetVector);
	void storeTargetVectorInSkill (TargetVector** targetVector, ENTITY* ent, int skillID);
	TargetVector* retrieveTargetVectorFromSkill (ENTITY* ent, int skillID);

	typedef struct TargetAngle
	{
		ANGLE current;
		ANGLE destination;
		BOOL linear;
		float blendFactor;
	} TargetAngle;

	TargetAngle* newTargetAngle (BOOL linear);
	void initTargetAngle (TargetAngle** a, ANGLE* aCurr, ANGLE* aTarget, float blend);
	void deleteTargetAngle (TargetAngle** targetAngle);
	void setTargetAngle (TargetAngle** targetAngle, ANGLE* current, ANGLE* destination);
	void setTargetAngleCurrent (TargetAngle** targetAngle, ANGLE* current);
	void setTargetAngleTarget (TargetAngle** targetAngle, ANGLE* destination);
	void setTargetAngleBlendFactor (TargetAngle** targetAngle, float factor);
	void moveTargetAngle (TargetAngle** targetAngle, float factor);
	ANGLE* getTargetAngleCurrent (TargetAngle** targetAngle);
	ANGLE* getTargetAngleDestination (TargetAngle** targetAngle);
	void storeTargetAngleInSkill (TargetAngle** targetAngle, ENTITY* ent, int skillID);
	TargetAngle* retrieveTargetAngleFromSkill (ENTITY* ent, int skillID);

// PRIVATE

	TargetValue* assertTargetValue (TargetValue** targetValue);
	TargetVector* assertTargetVector (TargetVector** targetVector);
	TargetAngle* assertTargetAngle (TargetAngle** targetAngle);

// Code include
#include "target.c"

#endif /* target_h */