#ifndef target_c
#define target_c

#include "target.h"

// TargetValue

	TargetValue* newTargetValue (BOOL linear)
	{
		TargetValue* targetValue = (TargetValue*)sys_malloc(sizeof(TargetValue));
		
		targetValue->current = 0.0;
		targetValue->destination = 0.0;
		targetValue->linear = linear;
		
		return(targetValue);
	}

	void deleteTargetValue (TargetValue** targetValue)
	{
		if (targetValue == NULL)
			return;

		if (*targetValue != NULL)
		{
			sys_free((void*)(*targetValue));
			*targetValue = NULL;
		}
	}

	void setTargetValue (TargetValue** targetValue, float current, float destination)
	{
		if (targetValue == NULL)
			return;
			
		assertTargetValue(targetValue);

		if (*targetValue != NULL) {
			(*targetValue)->current     = current;
			(*targetValue)->destination = destination;
		}
	}

	void setTargetValueCurrent (TargetValue** targetValue, float current)
	{
		if (targetValue == NULL)
			return;
			
		assertTargetValue(targetValue);

		if (*targetValue != NULL)
			(*targetValue)->current = current;
	}

	void setTargetValueTarget (TargetValue** targetValue, float destination)
	{
		if (targetValue == NULL)
			return;

		assertTargetValue(targetValue);

		if (*targetValue != NULL)
			(*targetValue)->destination = destination;
	}

	void setTargetValueBlendFactor (TargetValue** targetValue, float factor)
	{
		if (targetValue == NULL)
			return;

		assertTargetValue(targetValue);

		if (*targetValue != NULL)
			(*targetValue)->blendFactor = factor;
	}

	void moveTargetValue (TargetValue** targetValue, float factor)
	{
		if (targetValue == NULL)
			return;
			
		assertTargetValue(targetValue);

		if (*targetValue != NULL)
		{
			float movement = 0.0;
			if ((*targetValue)->linear) { // If value movement is linear
				float difference = abs((*targetValue)->destination - (*targetValue)->current);
				float movement   = minv(abs((*targetValue)->blendFactor * factor), difference) * sign((*targetValue)->destination - (*targetValue)->current);
			}
			else
				movement = (*targetValue)->blendFactor * factor * ((*targetValue)->destination - (*targetValue)->current); // Scaled difference

			(*targetValue)->current += movement;
		}
	}
	
	float getTargetValueCurrent (TargetValue** targetValue)
	{
		if (targetValue == NULL)  return(0);
		assertTargetValue(targetValue);
		if (*targetValue == NULL) return(0);
		return((*targetValue)->current);
	}

	float getTargetValueDestination (TargetValue** targetValue)
	{
		if (targetValue == NULL)  return(0);
		assertTargetValue(targetValue);
		if (*targetValue == NULL) return(0);
		return((*targetValue)->destination);
	}
	
	void storeTargetValueInSkill (TargetValue** targetValue, ENTITY* ent, int skillID)
	{
		if ((targetValue == NULL) || (ent == NULL)) // todo: skillID range check
			return;

		assertTargetValue(targetValue);
		if (*targetValue == NULL) return;

		(ent->skill)[skillID] = (void*)(*targetValue);
	}

	TargetValue* retrieveTargetValueFromSkill (ENTITY* ent, int skillID)
	{
		if (ent == NULL) // todo: skillID range check
			return;

		// todo: 0 check?
		return((TargetValue*)(void*)(ent->skill)[skillID]);
	}


// TargetVector

	void initTargetVector (TargetVector** t, VECTOR* vCurr, VECTOR* vTarget, float blend)
	{
		deleteTargetVector(t);
		setTargetVector(t, vCurr, vTarget);
		setTargetVectorBlendFactor(t, blend);
	}

	TargetVector* newTargetVector (BOOL linear)
	{
		TargetVector* targetVector = (TargetVector*)sys_malloc(sizeof(TargetVector));
		
		vec_set(&(targetVector->current),     nullvector);
		vec_set(&(targetVector->destination), nullvector);
		targetVector->linear = linear;

		return(targetVector);
	}

	void deleteTargetVector (TargetVector** targetVector)
	{
		if (targetVector == NULL)
			return;

		if (*targetVector != NULL)
		{
			sys_free((void*)(*targetVector));
			*targetVector = NULL;
		}
	}
	
	void setTargetVector (TargetVector** targetVector, VECTOR* current, VECTOR* destination)
	{
		if (targetVector == NULL)
			return;
			
		assertTargetVector(targetVector);

		if (*targetVector != NULL)
		{
			vec_set(&((*targetVector)->current), current);
			vec_set(&((*targetVector)->destination), destination);
		}
	}

	void setTargetVectorCurrent (TargetVector** targetVector, VECTOR* current)
	{
		if (targetVector == NULL)
			return;
			
		assertTargetVector(targetVector);

		if (*targetVector != NULL)
			vec_set(&((*targetVector)->current), current);
	}

	void setTargetVectorTarget (TargetVector** targetVector, VECTOR* destination)
	{
		if (targetVector == NULL)
			return;
			
		assertTargetVector(targetVector);

		if (*targetVector != NULL)
			vec_set(&((*targetVector)->destination), destination);
	}
	
	void setTargetVectorBlendFactor (TargetVector** targetVector, float factor)
	{
		if (targetVector == NULL)
			return;

		assertTargetVector(targetVector);

		if (*targetVector != NULL)
			(*targetVector)->blendFactor = factor;
	}

	void moveTargetVector (TargetVector** targetVector, float factor)
	{
		if (targetVector == NULL)
			return;
			
		assertTargetVector(targetVector);

		if (*targetVector != NULL)
		{
			if ((*targetVector)->linear) // If value movement is linear
			{
				float difference = vec_dist(&((*targetVector)->current), &((*targetVector)->destination));
				float movement   = minv(abs((*targetVector)->blendFactor * factor), difference);

				VECTOR moveVector;
				vec_diff(&moveVector, &((*targetVector)->destination), &((*targetVector)->current));
				vec_normalize(&moveVector, (var)movement);
				vec_add(&((*targetVector)->current), &moveVector);
			}
			else
				vecLerp((*targetVector)->current.x, (*targetVector)->current.x, (*targetVector)->destination.x, (*targetVector)->blendFactor * factor);
		}
	}

	VECTOR* getTargetVectorCurrent (TargetVector** targetVector)
	{
		if (targetVector == NULL)  return(0);
		assertTargetVector(targetVector);
		if (*targetVector == NULL) return(0);
		return(&((*targetVector)->current));
	}

	VECTOR* getTargetVectorDestination (TargetVector** targetVector)
	{
		if (targetVector == NULL)  return(0);
		assertTargetVector(targetVector);
		if (*targetVector == NULL) return(0);
		return(&((*targetVector)->destination));
	}
	
	void storeTargetVectorInSkill (TargetVector** targetVector, ENTITY* ent, int skillID)
	{
		if ((targetVector == NULL) || (ent == NULL)) // todo: skillID range check
			return;

		assertTargetVector(targetVector);
		if (*targetVector == NULL) return;

		ent->skill[skillID] = *targetVector;
	}

	TargetVector* retrieveTargetVectorFromSkill (ENTITY* ent, int skillID)
	{
		if (ent == NULL) // todo: skillID range check
			return;

		// todo: 0 check?
		return((TargetVector*)ent->skill[skillID]);
	}

// TargetAngle

	void initTargetAngle (TargetAngle** a, ANGLE* aCurr, ANGLE* aTarget, float blend)
	{
		deleteTargetAngle(a);
		setTargetAngle(a, aCurr, aTarget);
		setTargetAngleBlendFactor(a, blend);
	}

	TargetAngle* newTargetAngle (BOOL linear)
	{
		TargetAngle* targetAngle = (TargetAngle*)sys_malloc(sizeof(TargetAngle));
		
		vec_set(&(targetAngle->current),     nullvector);
		vec_set(&(targetAngle->destination), nullvector);
		targetAngle->linear = linear;

		return(targetAngle);
	}

	void deleteTargetAngle (TargetAngle** targetAngle)
	{
		if (targetAngle == NULL)
			return;

		if (*targetAngle != NULL)
		{
			sys_free((void*)(*targetAngle));
			*targetAngle = NULL;
		}
	}

	void setTargetAngle (TargetAngle** targetAngle, ANGLE* current, ANGLE* destination)
	{
		if (targetAngle == NULL)
			return;
			
		assertTargetAngle(targetAngle);

		if (*targetAngle)
		{
			vec_set(&((*targetAngle)->current), current);
			vec_set(&((*targetAngle)->destination), destination);
		}
	}

	void setTargetAngleCurrent (TargetAngle** targetAngle, ANGLE* current)
	{
		if (targetAngle == NULL)
			return;
			
		assertTargetAngle(targetAngle);

		if (*targetAngle)
			vec_set(&((*targetAngle)->current), current);
	}

	void setTargetAngleTarget (TargetAngle** targetAngle, ANGLE* destination)
	{
		if (targetAngle == NULL)
			return;
			
		assertTargetAngle(targetAngle);

		if (*targetAngle)
			vec_set(&((*targetAngle)->destination), destination);
	}
	
	void setTargetAngleBlendFactor (TargetAngle** targetAngle, float factor)
	{
		if (targetAngle == NULL)
			return;

		assertTargetAngle(targetAngle);

		if (*targetAngle != NULL)
			(*targetAngle)->blendFactor = factor;
	}

	void moveTargetAngle (TargetAngle** targetAngle, float factor)
	{
		if (targetAngle == NULL)
			return;

		assertTargetAngle(targetAngle);
		
		if (*targetAngle != NULL)
		{
			float fac = (*targetAngle)->blendFactor * factor;

			if ((*targetAngle)->linear)
			{
				// todo: ang_diff?
				float difference = vec_dist(&((*targetAngle)->destination), &((*targetAngle)->current));
				float movement = minv(abs(factor), difference);
				fac = movement / difference;
			}

			angLerp(&((*targetAngle)->current), &((*targetAngle)->current), &((*targetAngle)->destination), fac);
		}
	}
	
	ANGLE* getTargetAngleCurrent (TargetAngle** targetAngle)
	{
		if (targetAngle == NULL)  return(0);
		assertTargetAngle(targetAngle);
		if (*targetAngle == NULL) return(0);
		return(&((*targetAngle)->current));
	}

	ANGLE* getTargetAngleDestination (TargetAngle** targetAngle)
	{
		if (targetAngle == NULL)  return(0);
		assertTargetAngle(targetAngle);
		if (*targetAngle == NULL) return(0);
		return(&((*targetAngle)->destination));
	}
	
	void storeTargetAngleInSkill (TargetAngle** targetAngle, ENTITY* ent, int skillID)
	{
		if ((targetAngle == NULL) || (ent == NULL)) // todo: skillID range check
			return;

		assertTargetAngle(targetAngle);
		if (*targetAngle == NULL) return;

		(ent->skill)[skillID] = (void*)(*targetAngle);
	}

	TargetAngle* retrieveTargetAngleFromSkill (ENTITY* ent, int skillID)
	{
		if (ent == NULL) // todo: skillID range check
			return;

		// todo: 0 check?
		return((TargetAngle*)(void*)(ent->skill)[skillID]);
	}
	
// Assertions
	
	TargetValue* assertTargetValue (TargetValue** targetValue)
	{
		if (targetValue == NULL)
			return(NULL);
	
		if (*targetValue == NULL)
			*targetValue = newTargetValue(false);
	
		return(*targetValue);		
	}

	TargetVector* assertTargetVector (TargetVector** targetVector)
	{
		if (targetVector == NULL)
			return(NULL);
	
		if (*targetVector == NULL)
			*targetVector = newTargetVector(false);
	
		return(*targetVector);
	}

	TargetAngle* assertTargetAngle (TargetAngle** targetAngle)
	{
		if (targetAngle == NULL)
			return(NULL);
	
		if (*targetAngle == NULL)
			*targetAngle = newTargetAngle(false);
	
		return(*targetAngle);
	}

#endif /* target_c */