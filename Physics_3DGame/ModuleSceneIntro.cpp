#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	CreateWorld();  //mamausacions i altres mamausades 

	/*limit1.size.x = 2;
	limit1.size.y = 30;
	limit1.size.z = 400;
	limit1.color = Red;
	limit1.SetPos(100, 0, 0);
	limit1.wire = true;
	limit1.Render();*/

	App->physics->AddBody(limit1);

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (int i = 0; i < 2; i++)
	{
		FloorCubes[i]->Render();
	}

	for (int j = 0; j < 4; j++) 
	{
		LimitCubes[j]->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateWorld()
{
	BodySize FloorSize[2] = { {10, 1, 20}, {10, 1, 60} };

	vec3 FloorPosition[2] = { {-20, 1, 20}, {-30, 1, 50} };

	BodyRotation FloorRotation[2] = { {-45, {0,1,0}}, {0, {0,1,0}} };

	for (int i = 0; i < 2; i++)
	{
		FloorCubes.PushBack(new Cube(FloorSize[i].sizeX, FloorSize[i].sizeY, FloorSize[i].sizeZ));
		FloorCubes[i]->SetPos(FloorPosition[i].x, FloorPosition[i].y, FloorPosition[i].z);
		FloorCubes[i]->SetRotation(FloorRotation[i].angle, FloorRotation[i].axis_pos);
		bodyFloorCubes.PushBack(App->physics->AddBody(*(FloorCubes[i]), 0.0f));
	}

	BodySize LimitsSize[4] = { {2, 30, 400}, {2, 30, 400}, {400, 30 , 2}, {400, 30, 2} };
	vec3 LimitsPosition[4] = { {-200, 15, 0}, {200, 15, 0}, {0, 15, 200}, {0, 15, -200} };

	for (int j = 0; j < 4; j++) 
	{
		LimitCubes.PushBack(new Cube(LimitsSize[j].sizeX, LimitsSize[j].sizeY, LimitsSize[j].sizeZ));
		LimitCubes[j]->SetPos(LimitsPosition[j].x, LimitsPosition[j].y, LimitsPosition[j].z);
		bodyLimitCubes.PushBack(App->physics->AddBody(*(LimitCubes[j]), 0.0f));
	}
}

