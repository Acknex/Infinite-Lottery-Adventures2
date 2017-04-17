#include <acknex.h>
#include <default.c>

#include <mtlFX.c>
#include <mtlView.c>
#include <stdio.h>

#include "target.h"

SOUND* sndWalk = "sndWalk.ogg"; var hWalkLoop = 0;
SOUND* sndStand = "sndStand.ogg"; var hStandLoop = 0;
SOUND* sndJump = "sndJump.ogg";
SOUND* sndItem = "sndItem.ogg";
SOUND* sndPuke = "sndPuke.ogg";
SOUND* sndSpawn = "sndSpawn.ogg";
SOUND* sndDie = "sndDie.ogg";
SOUND* sndKill = "sndKill.ogg";
SOUND* sndJingle = "sndJingle.ogg";

SOUND* sndDarthDie = "sndDarthDie.ogg";
SOUND* sndDarthIdle = "sndDarthIdle.ogg";

SOUND* sndMusic = "music.ogg"; var hMusic = 0;

SOUND* sndTaunts [6];
SOUND* sndDarthTaunts [4];

ENTITY* collisionSphere = NULL;

#define ENEMY_AUDIO_RANGE 600

void loadSndArray (SOUND** sndArr, char* name, int num)
{
	int i;
	char b [256];

	for (i = 0; i < num; i++)
	{
		sprintf(b, "%s%d.ogg", name, i);
		sndArr[i] = snd_create(b);
	}
}

var sndArr_playRand (SOUND** sndArr, var volume, var balance, int num)
{
	return(snd_play(sndArr[(int)(random(num))], volume, balance));
}

var entSndArr_playRand (ENTITY* e, SOUND** sndArr, var volume, int num)
{
	return(ent_playsound(e, sndArr[(int)(random(num))], volume));
}


MATERIAL* mtlAlphatest =
{
  effect =
  "
    technique alpha_test
    {
      pass p0
      {
        zWriteEnable = true;
        alphaTestEnable = true;
        alphaBlendEnable = false;
      }
    }
  ";
}

action tree ()
{
	set(my, PASSABLE);
	my.material = mtlAlphatest;
}

PANEL* splash =
{
	layer = 1;
	bmap = "splash.tga";
	flags = SHOW | FILTER;
}

PANEL* splashAnykey =
{
	layer = 2;
	bmap = "pressAnyKey.tga";
}

BMAP* pPuke = "effPuke.tga";
BMAP* pPuke2 = "effPuke2.tga";
BMAP* pSpark = "effSpark.tga";
BMAP* pExterminate = "inf.tga";

void effActorPuke_p (PARTICLE* p);
void effActorPuke (ENTITY* e)
{
	var total = 48;
	var timer = 0;
	var timer_reset = 1;

	while (total > 0)
	{
		total -= time_step;
		timer -= time_step;

		if (timer < 0)
		{
			timer = timer_reset;

			VECTOR v, a;
			vec_for_vertex(&v, e, 544);

			ANGLE ang;
			ang.pan = e->pan + (-20 + random(40));
			ang.tilt = ang.roll = 0;
			vec_for_angle(&a, &ang);

			effect(effActorPuke_p, 1, &v, &a);
		}

		wait(1);
	}
}
	void effActorPuke_ev (PARTICLE* p);
	void effActorPuke_p (PARTICLE* p)
	{
		if (random(100) < 50)
			p->bmap = pPuke;
		else
			p->bmap = pPuke2;

		p->flags |= TRANSLUCENT;

		p->alpha = 30 + random(50);
		p->size = 8 + random(16);

		p->event = effActorPuke_ev;
	}

		void effActorPuke_ev (PARTICLE* p)
		{
			p->x += p->vel_x * 8 * time_step;
			p->y += p->vel_y * 8 * time_step;
			p->z -= 2 * time_step;

			p->alpha -= 2 * time_step;
			if (p->alpha < 0) p->alpha = 0;

			p->lifespan = (p->alpha != 0) * 100;
		}

void effSpark_ev (PARTICLE* p);
void effSpark_p (PARTICLE* p)
{
	p->bmap = pSpark;

	p->flags |= TRANSLUCENT;

	p->alpha = 15 + random(35);
	p->size = 16 + random(64);

	p->vel_x = -5 + random(10);
	p->vel_y = -5 + random(10);

	p->skill_a = 1 + random(2); // gravity
	p->skill_b = 0.5; // fadeout speed
	p->skill_d = random(10); // push up speed

	p->event = effSpark_ev;
}

	void effSpark_ev (PARTICLE* p)
	{
		p->alpha -= p->skill_b * time_step;
		if (p->alpha < 0) p->alpha = 0;
	
		p->x += p->vel_x * time_step;
		p->y += p->vel_x * time_step;
	
		p->z += p->skill_d * time_step;
	
		p->z -= p->skill_a * time_step;
		if (p->skill_d > 0)
			p->skill_d = maxv(0, p->skill_d - p->skill_a * time_step);
	
		// When alpha is zero, remove particle
		p->lifespan = (p->alpha != 0) * 100;
	}
	
void effExterminate_ev (PARTICLE* p);
void effExterminate_p (PARTICLE* p)
{
	p->bmap = pExterminate;

	p->flags |= TRANSLUCENT;

	p->alpha = 15 + random(35);
	p->size = 16 + random(64);

	p->vel_x = -5 + random(10);
	p->vel_y = -5 + random(10);

	p->skill_a = 1 + random(2); // gravity
	p->skill_b = 0.5; // fadeout speed
	p->skill_d = random(10); // push up speed
	
	vec_set(p->blue, vector(random(255), random(255), random(255)));

	p->event = effExterminate_ev;
}

	void effExterminate_ev (PARTICLE* p)
	{
		p->alpha -= p->skill_b * time_step;
		if (p->alpha < 0) p->alpha = 0;
	
		p->x += p->vel_x * time_step;
		p->y += p->vel_x * time_step;
	
		p->z += p->skill_d * time_step;
	
		p->z -= p->skill_a * time_step;
		if (p->skill_d > 0)
			p->skill_d = maxv(0, p->skill_d - p->skill_a * time_step);
	
		// When alpha is zero, remove particle
		p->lifespan = (p->alpha != 0) * 100;
	}
	

void effDie (ENTITY* e, int res)
{
	int i; VECTOR v;
	for (i = ent_vertices(e); i > 0; i--)
	{
		if (i % res == 0)
		{
			vec_for_vertex(&v, e, i);
			effect(effSpark_p, 1, &v, nullvector);
		}
	}
}

void effExterminate (ENTITY* e, int res)
{
	int i; VECTOR v;
	for (i = ent_vertices(e); i > 0; i--)
	{
		if (i % res == 0)
		{
			vec_for_vertex(&v, e, i);
			effect(effExterminate_p, 1, &v, nullvector);
		}
	}
}

ENTITY* ptrActor = NULL;

#define JUMP_FORCE skill1

#define CAM_TILT skill2
#define CAM_PAN skill3
#define CAM_DIST skill4
#define CAM_T_POS skill5
#define CAM_T_ANG skill6
#define CAM_T_ANG_S skill7
#define ACTOR_DIST_GROUND skill8

#define ANIM_WALK_CNT skill8
#define ANIM_WALK_SPEED 11

#define ANIM_STAND_CNT skill9
#define ANIM_STAND_SPEED 2

#define ANIM_JUMP_CNT skill10
#define ANIM_JUMP_SPEED 3

#define ANIM_DIE_SPEED 2

#define ACTOR_PUKE_ANNOUNCED skill11
#define ACTOR_PUKE_ACTIVE skill12
#define ACTOR_PUKE_ANIM_CNT skill13
#define ANIM_PUKE_SPEED 3

#define ACTOR_ALIVE skill14

#define ACTOR_HEALTH skill15
#define ACTOR_HEALTH_INIT 3

#define ACTOR_TURN_SPEED 15
#define ACTOR_WALK_SPEED 20

#define ACTOR_JUMP_FORCEUP 50
#define ACTOR_JUMP_GRAVITY 5
#define GRAVITY -20

#define ACTOR_PAN skill16

#define TYPE skill66
	#define TYPE_ELSE 0
	#define TYPE_LOTTI 1
	#define TYPE_ENEMY 2
	
#define ENEMY_DIE_NOW skill67

#define CAM_TILT_SPEED 10
#define CAM_TILT_MIN 10
#define CAM_TILT_MAX 45

#define CAM_DIST_SPEED 2
#define CAM_DIST_MIN 100
#define CAM_DIST_MAX 300

#define ACTOR_DIE_ARC_SPEED 0.5
#define ACTOR_DIE_ARC_TIME 4 // sec
#define ACTOR_DIE_EFF_RES 4
#define ACTOR_DIE_LOOKAT_VERTEX 544
#define ACTOR_DIE_OFF_X 25
#define ACTOR_DIE_OFF_Y 25
#define ACTOR_DIE_OFF_Z 50

#define TAUNT_TIME_MIN 20
#define TAUNT_TIME_MAX 60
var tauntTimer = 640; // 40s

var lifes = 42;
var points = 0;

void cheatLifes ()
{
	if (lifes > 3)
		lifes = 3;
}

void updateTaunt ()
{
	tauntTimer -= time_step;
	if (tauntTimer <= 0)
	{
		tauntTimer = (TAUNT_TIME_MIN + random(TAUNT_TIME_MAX - TAUNT_TIME_MIN)) * 16;
		sndArr_playRand(sndTaunts, 100, 0, 6);
	}
}

void forceTaunt ()
{
	tauntTimer = -1;
	updateTaunt();
}

void spawnLotti ();

function actor_event() 
{
	switch (event_type)
	{
		case EVENT_ENTITY:	if ((you.TYPE == TYPE_ENEMY) && (you.ENEMY_DIE_NOW < 1))
							{
								if (my.z > (you.z + abs(you.min_z)))
								{
									you.ENEMY_DIE_NOW = 1;
									points += 15;
								}
								else
								{
									my.ACTOR_ALIVE = 0;
									entSndArr_playRand(you, sndDarthTaunts, ENEMY_AUDIO_RANGE, 4);
									points -= 12;
								}
							}
							
							return;
	}
}  

void actor ()
{
	my.TYPE = TYPE_LOTTI;
	my.ACTOR_ALIVE = 1;
	
	my.ACTOR_HEALTH = ACTOR_HEALTH_INIT;	
	
	
	my.material = mtl_specParallax;
	
	shadow_stencil = 2;
	set(my, SHADOW | CAST);
	
	my.CAM_TILT = CAM_TILT_MIN + (CAM_TILT_MAX - CAM_TILT_MIN)/2;
	my.CAM_PAN = my.pan + 180;
	my.CAM_DIST = CAM_DIST_MIN + (CAM_DIST_MAX - CAM_DIST_MIN)/2;
	
	initTargetVector(&(my->CAM_T_POS), my.x, my.x, 0.1); // position
	initTargetAngle(&(my->CAM_T_ANG), my.pan, my.pan, 0.2); // angle
	
	set(my, INVISIBLE);
	
	// cam init
	{
		VECTOR v;
		
		camera.arc = 60;
		
		vec_set(&v, vector(100, 0, 50));
		vec_rotate(&v, my.pan);
		vec_add(&v, my.x);
		vec_set(camera.x, &v);
		
		vec_diff(&v, my.x, camera.x);
		vec_to_angle(camera.pan, &v);
		camera.roll = 0;
	}
	
	wait(-1);
	
	snd_play(sndSpawn, 100, 0);
	
	effDie(my, ACTOR_DIE_EFF_RES);
	
	reset(my, INVISIBLE);
	
	float s = 0.1;
	while (s < 1)
	{
		s += 0.05 * time_step;
		if (s > 1)
			s = 1;
			
		my.scale_x = my.scale_y = my.scale_z = s;
		
		ent_animate(my, "jump", 100 * s, 0);
		
		wait(1);
	}
	
	// easteregg!
	on_3 = cheatLifes;
	on_t = forceTaunt;

	ptrActor = my;
	
	my.scale_x = my.scale_y = my.scale_z = 1;
	c_updatehull(my, 2);
	c_setminmax(my);	
	vec_scale(my->min_x, 0.75);
	vec_scale(my->max_x, 0.75);
	
	my.emask |= (ENABLE_ENTITY);
	my.event = actor_event;
	
	my.trigger_range = 50;
	
	while (my.ACTOR_ALIVE > 0)
	{
		
		// taunt
		if (my.ACTOR_PUKE_ACTIVE < 1)
			updateTaunt();

		// rotation
		if (my.ACTOR_PUKE_ACTIVE < 1)
		{
			ANGLE rot;
			rot.tilt = rot.roll = 0;
			rot.pan = -mouse_force.x * ACTOR_TURN_SPEED * time_step;

			c_rotate(my, &rot, GLIDE | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_SPRITES);
			
			vec_set(getTargetAngleDestination(&(my->ACTOR_PAN)), vector(my.pan, 0, 0));
		}

		// jump
		if (my.ACTOR_PUKE_ACTIVE < 1)
		{
			if ((my.JUMP_FORCE <= 0) && (key_space))
			{
				my.JUMP_FORCE = ACTOR_JUMP_FORCEUP;
				snd_play(sndJump, 100, 0);
			}
			else
			{
				my.JUMP_FORCE = clamp(my.JUMP_FORCE - ACTOR_JUMP_GRAVITY * time_step, 0, my.JUMP_FORCE);
				
				if ((my.JUMP_FORCE <= 0) && (my.ACTOR_PUKE_ANNOUNCED))
				{
					my.ACTOR_PUKE_ANNOUNCED = 0;
					my.ACTOR_PUKE_ACTIVE = 1;

					snd_play(sndPuke, 100, 0);
					effActorPuke(my);
				}
			}
		}
		
		// momentum
		VECTOR diff;
		
		if (my.ACTOR_PUKE_ACTIVE < 1)
		{
			VECTOR oldPosition;
			vec_set(&oldPosition, my.x);
			
			VECTOR momentum;
			momentum.x = key_w - key_s;
			momentum.y = -key_d + key_a; 
			momentum.z = 0;
			vec_normalize(&momentum, time_step * ACTOR_WALK_SPEED);
			
			VECTOR gravity;
			gravity.x = gravity.y = 0;
			gravity.z = (GRAVITY + my.JUMP_FORCE) * time_step;
			
			c_move(my, &momentum, &gravity, GLIDE | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_SPRITES | ACTIVATE_TRIGGER);
			
			vec_diff(&diff, my.x, &oldPosition);
		}
		
		// camera
		{
			camera.arc = 80;
			
			VECTOR l, v;
			
			my.CAM_TILT += mouse_force.y * CAM_TILT_SPEED * time_step;
			my.CAM_TILT = clamp(ang(my.CAM_TILT), CAM_TILT_MIN, CAM_TILT_MAX);
			
			my.CAM_DIST += mickey.z * CAM_DIST_SPEED * time_step;
			my.CAM_DIST = clamp(my.CAM_DIST, CAM_DIST_MIN, CAM_DIST_MAX);
			
			// cam move
			{
				
				if (vec_length(&diff) > 0)
				{
					my.CAM_PAN = my.pan + 180;
				}

				if (my.ACTOR_PUKE_ACTIVE)
				{
					my.CAM_PAN = my.pan;
					my.CAM_DIST = CAM_DIST_MIN;
				}
				
				ANGLE a;
				a.pan = my.CAM_PAN;
				a.tilt = my.CAM_TILT;
				a.roll = 0;
				
				vec_for_angle(&l, &a);
				vec_normalize(&l, my.CAM_DIST);
				vec_add(&l, my.x);
				
				reset(collisionSphere, PASSABLE);
				
				VECTOR dv; vec_diff(&dv, &l, my.x);
				you = my;
				vec_set(collisionSphere->x, my.x);
				
				c_move(collisionSphere, nullvector, &dv, IGNORE_YOU | GLIDE | IGNORE_PASSABLE | USE_POLYGON);
				set(collisionSphere, PASSABLE);
				
				vec_set(getTargetVectorDestination(&(my->CAM_T_POS)), collisionSphere->x);
				
				moveTargetVector(&(my->CAM_T_POS), time_step);
				vec_set(camera->x, getTargetVectorCurrent(&(my->CAM_T_POS)));
			}
			
			// cam look at
			{
				VECTOR v;
				ANGLE a;
				vec_set(&v, my.x);
				
				if (vec_length(&diff) > 0)
				{
					vec_set(&v, vector(75,0,0));
					vec_rotate(&v, my.pan);
					vec_add(&v, my.x);
				}
				
				vec_diff(&v, &v, getTargetVectorDestination(&(my->CAM_T_POS)));
				vec_to_angle(&a, &v);
				a.roll = 0;
				
				vec_set(getTargetAngleDestination(&(my->CAM_T_ANG)), &a);
				moveTargetAngle(&(my->CAM_T_ANG), time_step);
				vec_set(camera->pan, getTargetAngleCurrent(&(my->CAM_T_ANG)));
			}
		}
		
		// animation
		{
			my.ANIM_WALK_CNT += ANIM_WALK_SPEED * time_step;
			my.ANIM_STAND_CNT += ANIM_STAND_SPEED * time_step;
			my.ANIM_JUMP_CNT += ANIM_JUMP_SPEED * time_step;

			if (hWalkLoop == 0)
				hWalkLoop = snd_loop(sndWalk, 5, 0);
			snd_pause(hWalkLoop);
				
			if (hStandLoop == 0)
				hStandLoop = snd_loop(sndStand, 5, 0);
			snd_pause(hStandLoop);

			if (my.ACTOR_PUKE_ACTIVE > 0)
			{
				my.ACTOR_PUKE_ANIM_CNT += ANIM_PUKE_SPEED * time_step;
				my.ACTOR_PUKE_ANIM_CNT = clamp(my.ACTOR_PUKE_ANIM_CNT, 0, 100);

				ent_animate(my, "puke", my.ACTOR_PUKE_ANIM_CNT, 0);

				if (my.ACTOR_PUKE_ANIM_CNT >= 100)
				{
					my.ACTOR_PUKE_ACTIVE = 0;
					my.ACTOR_PUKE_ANIM_CNT = 0;
				}
			}
			else
			{
				if (my.JUMP_FORCE > 0)
				{
					ent_animate(my, "jump", my.ANIM_JUMP_CNT, ANM_CYCLE);
				}
				else
				{
					if (vec_length(&diff) > 0)
					{
						ent_animate(my, "walk", my.ANIM_WALK_CNT, ANM_CYCLE);
						snd_start(hWalkLoop);
					}
					else
					{
						ent_animate(my, "stand", my.ANIM_WALK_CNT, ANM_CYCLE);
						snd_start(hStandLoop);
					}
				}
			}
		}

		if ((my.ACTOR_HEALTH <= 0) || (key_k))
		{
			my.ACTOR_ALIVE = 0;
		}

		wait(1);
	}
	
	snd_play(sndDie, 100, 0);
	
	snd_pause(hWalkLoop);
	snd_pause(hStandLoop);
	snd_stop(hWalkLoop);	
	snd_stop(hStandLoop);
	hWalkLoop = hStandLoop = 0;

	var animDie = 0;
	while (animDie < 100)
	{
		// cam
		{
			// position

				VECTOR p;
				vec_set(&p, my.x);
				vec_add(&p, vector(ACTOR_DIE_OFF_X, ACTOR_DIE_OFF_Y, ACTOR_DIE_OFF_Z));

				vec_set(getTargetVectorDestination(&(my->CAM_T_POS)), &p);

				moveTargetVector(&(my->CAM_T_POS), time_step);
				vec_set(camera->x, getTargetVectorCurrent(&(my->CAM_T_POS)));

			// lookat
			
				vec_for_vertex(&p, my, ACTOR_DIE_LOOKAT_VERTEX);

				ANGLE a;
				vec_diff(&p, &p, getTargetVectorDestination(&(my->CAM_T_POS)));
				vec_to_angle(&a, &p);
				a.roll = 0;

				vec_set(getTargetAngleDestination(&(my->CAM_T_ANG)), &a);
				moveTargetAngle(&(my->CAM_T_ANG), time_step);
				vec_set(camera->pan, getTargetAngleCurrent(&(my->CAM_T_ANG)));
		}

		// animation
		animDie += ANIM_DIE_SPEED * time_step;
		animDie = clamp(animDie, 0, 100);

		ent_animate(my, "die", animDie, 0);

		camera.arc += ACTOR_DIE_ARC_SPEED * time_step;

		wait(1);
	}
	
	if (lifes > 1)
	{
		// Todeseffekt, dümm dümm dü dü dü dümm dümmm
		{
			effDie(my, ACTOR_DIE_EFF_RES);
			set(my, INVISIBLE);
			reset(my, SHADOW);
		}
		
		lifes -= 1;
	
		snd_play(sndJingle, 50, 0); // jingle
	
		var t = ACTOR_DIE_ARC_TIME * 16;
		while (t > 0)
		{
			camera.arc += ACTOR_DIE_ARC_SPEED * time_step;
			t -= time_step;
			wait(1);
		}
		
		wait(1);
	
		ptrActor = NULL;
		ptr_remove(my);
		
		spawnLotti();
	}
	else // Leben alle
	{
		snd_stop(hMusic);
		
		// KAPUTT!!!
		effExterminate(my, ACTOR_DIE_EFF_RES);
		set(my, INVISIBLE);
		reset(my, SHADOW);
		
		var h = snd_play(sndJingle, 50, 0); // jingle
		
		var t = ACTOR_DIE_ARC_TIME * 16;
		while ((t > 0) || (snd_playing(h)))
		{
			camera.arc += ACTOR_DIE_ARC_SPEED * time_step;
			t -= time_step;
			wait(1);
		}		
		
		wait(1);
		
		var hRibanna = media_play("ribanna.mid", NULL, 100);
		
		camera.alpha = 50;
		set(camera, TRANSLUCENT);
		while (1)
		{
			camera.pan += 1 * time_step;
			camera.tilt -= 1 * time_step;
			camera.roll -= 1 * time_step;
			camera.z += 3 * time_step;
			wait(1);
		}
	}
}

VECTOR spawnPoint;
ANGLE spawnAngle;
char spawnFile [512];

void spawnLotti ()
{
	wait(3);
	
	ENTITY* e = ent_create(spawnFile, &spawnPoint, actor);
	vec_set(e->pan, &spawnAngle);
}

action lotti ()
{
	vec_set(&spawnPoint, my.x);
	vec_set(&spawnAngle, my.pan);

	strcpy(spawnFile, my->type);

	ptr_remove(my);

	spawnLotti();
}



action darthLotti ()
{
	my.TYPE = TYPE_ENEMY;
	
	vec_scale(my->scale_x, 0.75);
	c_updatehull(my, 2);
	c_setminmax(my);	
	
	set(my, SHADOW);
	
	initTargetAngle(&(my->ACTOR_PAN), my.pan, my.pan, 0.2); // angle
	
	var animWalk = 0;
	
	var hIdle = ent_playloop(my, sndDarthIdle, ENEMY_AUDIO_RANGE);
	
	while (my.ENEMY_DIE_NOW < 1)
	{
		
		vec_set(getTargetAngleDestination(&(my->ACTOR_PAN)), vector(random(360), 0, 0));
		
		var dist = 200 + random(500);
		var standTime = (1 + random(3)) * 16;
		
		while (my.ENEMY_DIE_NOW < 1)
		{
			moveTargetAngle(&(my->ACTOR_PAN), time_step);
			vec_set(my->pan, getTargetAngleCurrent(&(my->ACTOR_PAN)));
			
			VECTOR diff; vec_set(&diff, nullvector);
			if (dist > 0)
			{
				VECTOR oldPosition;
				vec_set(&oldPosition, my.x);
				
				VECTOR v;
				vec_set(&v, vector(ACTOR_WALK_SPEED, 0, 0));
				vec_scale(&v, time_step);
				
				VECTOR gravity;
				gravity.x = gravity.y = 0;
				gravity.z = GRAVITY * time_step;
				
				c_move(my, &v, &gravity, GLIDE | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_SPRITES);
				
				vec_diff(&diff, my.x, &oldPosition);
				
				dist -= vec_length(&v);
			}
			else
			{
				standTime -= time_step;
				if (standTime < 0)
					break;
			}
			
			// animation
			{
				my.ANIM_WALK_CNT += ANIM_WALK_SPEED * time_step;
				my.ANIM_STAND_CNT += ANIM_STAND_SPEED * time_step;
	
				if (vec_length(&diff) > 2)
				{
					ent_animate(my, "walk", my.ANIM_WALK_CNT, ANM_CYCLE);
					snd_tune(hWalkLoop, 100, 0, 0);
				}
				else
				{
					ent_animate(my, "stand", my.ANIM_WALK_CNT, ANM_CYCLE);
					snd_tune(hStandLoop, 100, 0, 0);
				}
			}			
			
			wait(1);
		}
		
		wait(1);
	}
	
	snd_stop(hIdle);
	
	ent_playsound(my, sndKill, ENEMY_AUDIO_RANGE);
	
	set(my, PASSABLE);
	
	ent_playsound(my, sndDarthDie, ENEMY_AUDIO_RANGE);
	
	var animDie = 0;
	while (animDie < 100)
	{
		// animation
		animDie += ANIM_DIE_SPEED * time_step;
		animDie = clamp(animDie, 0, 100);

		ent_animate(my, "die", animDie, 0);

		wait(1);
	}
	
	effExterminate(my, 4);
	
	wait(1);
	
	ptr_remove(my);
}

#define ITEM_ALIVE skill1
#define ITEM_COLLECT_DISTANCE 80
#define ITEM_COLLECT_SPEED 10
#define ITEM_COLLECT_SCALE_DOWN 0.1
#define ITEM_COLLECT_SCALE_MIN 0.1

#define ITEM_NO_PLACE skill73

#define ITEM_POINTS skill77

void item ()
{
	set(my, PASSABLE);	
	my.ITEM_ALIVE = 1;
	
	wait(1);
	
	// placement
	if (my.ITEM_NO_PLACE < 1)
	{
		VECTOR t;
		vec_set(&t, my.x);
		t.z = -10000;
		
		c_trace(my.x, &t, IGNORE_ME | IGNORE_PASSABLE | IGNORE_SPRITES);
		
		my.z = hit.z + (abs(my.min_z) + abs(my.max_z)) * 1.5;		
	}
	
	while (my.ITEM_ALIVE > 0)
	{
		if (ptrActor)
		{
			if (vec_dist(my->x, ptrActor->x) < ITEM_COLLECT_DISTANCE)
			{
				my.ITEM_ALIVE = 0;
			}
		}
		
		wait(1);
	}
	
	snd_play(sndItem, 100, 0);
	
	points += my.ITEM_POINTS;
	
	while (my.scale_x > ITEM_COLLECT_SCALE_MIN)
	{
		if (ptrActor == NULL)
			break;
			
		my.scale_y = my.scale_z = my.scale_x = maxv(my.scale_x - ITEM_COLLECT_SCALE_DOWN * time_step, ITEM_COLLECT_SCALE_MIN);
		
		VECTOR v;
		vec_diff(&v, ptrActor->x, my->x);
		vec_normalize(&v, ITEM_COLLECT_SPEED * time_step);
		
		vec_add(my.x, &v);
		
		wait(1);
	}
	
	ptr_remove(my);
}

#define BULB_TILT_SPEED 2
#define BULB_TILT_VAR 25
#define BULB_PAN_SPEED 5

action bulb ()
{
	item();
	
	my.ITEM_POINTS = 1;
	
	my.skill1 = random(360);

	while (my.ITEM_ALIVE > 0)
	{
		my.tilt = sin(BULB_TILT_SPEED * (total_ticks + my.skill1)) * BULB_TILT_VAR;
		my.pan += BULB_PAN_SPEED * time_step;
		
		wait(1);
	}
}

action bulbStatic ()
{
	my.ITEM_NO_PLACE = 1;
	bulb();
}

#define USER_PAN_SPEED 5
#define USER_TILT_SPEED 5
#define USER_ROLL_SPEED 5

action user ()
{
	item();
	
	reset(my, OVERLAY);
	
	my.ITEM_POINTS = 100;

	my.skill1 = random(360);

	my.pan = random(360);
	my.tilt = random(360);
	my.roll = random(360);

	while (my.ITEM_ALIVE > 0)
	{
		my.pan += USER_PAN_SPEED * time_step;
		my.tilt += USER_TILT_SPEED * time_step;
		my.roll += USER_ROLL_SPEED * time_step;

		wait(1);
	}
}

#define COLL_TILT_SPEED 4
#define COLL_TILT_VAR 10
#define COLL_PAN_SPEED 2.5

#define COLL_IS_A_PUKE_REASON skill33

action coll ()
{
	item();
	
	my.skill1 = random(360);

	while (my.ITEM_ALIVE > 0)
	{
		my.tilt = sin(COLL_TILT_SPEED * (total_ticks + my.skill1)) * COLL_TILT_VAR;
		my.pan += COLL_PAN_SPEED * time_step;
		
		wait(1);
	}
	
	if (my.COLL_IS_A_PUKE_REASON > 0)
	{
		if (ptrActor != NULL)
		{
			ptrActor->ACTOR_PUKE_ANNOUNCED = 1;
			ptrActor->ACTOR_HEALTH -= 1;
		}
	}
}

action bug ()
{
	my.ITEM_POINTS = 10;
	
	coll();
}

action bugfix ()
{
	my.ITEM_POINTS = -10;
	
	my.COLL_IS_A_PUKE_REASON = 1;
	coll();
}

action unity ()
{
	my.ITEM_POINTS = -20;
	
	my.COLL_IS_A_PUKE_REASON = 1;
	coll();
}

action torque ()
{
	my.ITEM_POINTS = -20;
	
	my.COLL_IS_A_PUKE_REASON = 1;
	coll();
}

void skychange ()
{
	COLOR oldColor, newColor;
	var factor = 0.0;
	
	vec_set(oldColor.blue, screen_color.blue);
	vec_set(newColor.blue, vector(random(255), random(255), random(255)));
	
	while (1)
	{
		factor += 0.2 * time_step;
		factor = clamp(factor, 0.0, 1.0);
		
		vec_lerp(screen_color.blue, oldColor.blue, newColor.blue, factor);
		vec_set(sky_color.blue, screen_color.blue);
		
		if (factor == 1.0)
		{
			factor = 0.0;
			vec_set(oldColor.blue, screen_color.blue);
			vec_set(newColor.blue, vector(random(255), random(255), random(255)) );
		}	
		
		wait(1);
	}
}

float hdrThreshold = 20, hdrStrength = 50;

void hdr ()
{
	pp_set(camera,mtl_hdr);
	
	while (1)
	{
		mtl_hdr.skill1 = floatv(hdrStrength);
		mtl_hdr.skill2 = floatv(hdrThreshold);
		wait(1);
	}
}

#define MUSIC_VOL 40

#include "quote.h"

int main ()
{
	video_switch(8, 0, 1);
	
	hMusic = snd_loop(sndMusic, 40, 0);
	
	loadSndArray(sndTaunts, "sndTaunt", 6);
	loadSndArray(sndDarthTaunts, "sndDarthTaunt", 4);
	
	splash->scale_x = screen_size.x / splash->size_x;
	splash->scale_y = screen_size.y / splash->size_y;
	
	wait(3);
	
	freeze_mode = 1;
	level_load("level1b.wmb");
	
	d3d_fogcolor1.red = 64;
	d3d_fogcolor1.green = 32;
	d3d_fogcolor1.blue = 32;
	camera.fog_start = 100;
	camera.fog_end = 5000;
	fog_color = 1;
	
	collisionSphere = ent_create(SPHERE_MDL, nullvector, NULL);
	set(collisionSphere, INVISIBLE | PASSABLE);
	
		stencil_blur(1);
	
		camera->ambient = 15;
	
		vec_set(camera->x,   vector(-101, -185, 118));
		vec_set(camera->pan, vector(30, -31, 0));
	
		hdr();
		skychange();
		create_quotes();
	
	splashAnykey->pos_x = screen_size.x / 2 - splashAnykey->size_x / 2;
	splashAnykey->pos_y = screen_size.y * 0.75;
	set(splashAnykey, SHOW);
	
	while (!key_any) wait(1);
	while (key_any) wait(1);
	
	// flacker flacker
	var t = 16;
	fps_max = 24;
	while (t > 0)
	{
		t -= time_step;
		
		if (total_frames % 2 == 0)
			set(splashAnykey, SHOW);
		else
			reset(splashAnykey, SHOW);
		
		wait(1);
	}
	fps_max = 120;
	
	reset(splash, SHOW);
	reset(splashAnykey, SHOW);
	
	freeze_mode = 0;
	
	wait(3);
	
	while (1)
	{
		char b [256];
		
		if ((points >= 0) && (points < 1))
			sprintf(b, "lifes: %d points: 0.42", (int)lifes);
		else
			sprintf(b, "lifes: %d points: %d", (int)lifes, (int)points);
		
		draw_textmode("Times", 1, 40, 100);
		draw_text(b, 20, 20, vector(255, 214, 255));
		
		wait(1);
	}
}