#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f
#define INITIAL_POS 380, 2, 0
#define CHECKPOINT1 380, 2, 25
#define CHECKPOINT2 183, 2, 40

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

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;

	btTransform newCarPos;
	vec3 initialCarPos;
	vec3 newCarDirection;
	vec3 newCameraPos;
	vec3 viewCar;

	int laps = 0;
	int laps_time = 0;
	Timer timer;

	bool time_started = false;
	bool checkpoint1 = false;
	bool checkpoint2 = false;

	mat4x4 initialPosMatrix;
	mat4x4 checkpoint1Matrix;
	mat4x4 checkpoint2Matrix;

	
};