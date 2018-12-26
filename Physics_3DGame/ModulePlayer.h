#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1400.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f
#define INITIAL_POS 380, 2, 0
#define CHECKPOINT1 380, 2, 25
#define CHECKPOINT2 183, 2, 40
#define CHECKPOINT3 -205, 2, -120
#define CHECKPOINT4 -260, 2, 270
#define CHECKPOINT5 264, 2, 107
#define CHECKPOINT6 316, 2, -225
#define CHECKPOINT7 -150, 2, -263

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void CameraToPlayer();

	void MySetPos(vec3 newPos);
	void Respawn(vec3 respawn_pos);

	void ChooseMatrix(int num);

public:

	PhysVehicle3D*	vehicle;
	float			turn;
	float			acceleration;
	float			brake;

	btTransform		newCarPos;
	vec3			initialCarPos;
	vec3			newCarDirection;
	vec3			newCameraPos;
	vec3			viewCar;

	int				laps = 0;
	int				laps_time = 0;
	char*			message;
	Timer			timer;

	bool			time_started	= false;
	bool			checkpoint1		= false;
	bool			checkpoint2		= false;
	bool			checkpoint3		= false;
	bool			checkpoint4		= false;
	bool			checkpoint5		= false;
	bool			checkpoint6		= false;
	bool			checkpoint7		= false;
	bool			win				= false;
	bool			lose			= false;

	mat4x4			initialPosMatrix;
	mat4x4			checkpoint1Matrix;
	mat4x4			checkpoint2Matrix;
	mat4x4			checkpoint3Matrix;
	mat4x4			checkpoint4Matrix;
	mat4x4			checkpoint5Matrix;
	mat4x4			checkpoint6Matrix;
	mat4x4			checkpoint7Matrix;
	
};