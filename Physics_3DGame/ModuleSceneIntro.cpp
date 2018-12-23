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

	clock.Start();

	timer = { 0, 0, 2000, 150 };
	time_left = { 0, 0, time_toMove, 75 };

	CreateWorld();

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
	
	time_toMove = 2000 - clock.Read() * 11000;

	// 3 minutes to complete the level
	if (clock.Read() >= 1800000)
	{
		return UPDATE_ERROR;
	}

	for (int i = 0; i < num_floors; i++)
	{
		FloorCubes[i]->Render();
	}

	for (int i = 0; i < num_limits; i++) 
	{
		LimitCubes[i]->Render();
	}

	for (int i = 0; i < num_roads; i++)
	{
		RoadCubes[i]->Render();
	}

	for (int i = 0; i < num_buildings; i++)
	{
		BuildingsCubes[i]->Render();
	}

	for (int i = 0; i < num_details; i++)
	{
		DetailsCubes[i]->Render();
	}

	for (int i = 0; i < num_people; i++)
	{
		PeopleCubes[i]->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateWorld()
{
	// Rendering floor and sky
	FloorsRender();

	// Rendering map limits
	LimitsRender();

	// Rendering the road limits
	RoadRender();

	// Rendering buildings and people
	BuildingsRender();
	PeopleRender();

	// Rendering other details
	DetailsRender();
}

void ModuleSceneIntro::FloorsRender() 
{
	num_floors = 2;

	BodySize FloorSize[2] = { {800, 1, 800}, {800, 1, 800} };

	vec3 FloorPosition[2] = { {0, 0, 0}, {0, 80, 0} };

	//BodyRotation FloorRotation[2] = { {-45, {0,1,0}}, {0, {0,1,0}} };

	for (int i = 0; i < num_floors; i++)
	{
		FloorCubes.PushBack(new Cube(FloorSize[i].sizeX, FloorSize[i].sizeY, FloorSize[i].sizeZ));
		FloorCubes[i]->SetPos(FloorPosition[i].x, FloorPosition[i].y, FloorPosition[i].z);
		//FloorCubes[i]->SetRotation(FloorRotation[i].angle, FloorRotation[i].axis_pos);
		if (i == 0) { FloorCubes[i]->color = Grey2; }
		else if (i == 1) { FloorCubes[i]->color = Blue; }
		bodyFloorCubes.PushBack(App->physics->AddBody(*(FloorCubes[i]), 0.0f));
	}
}

void ModuleSceneIntro::LimitsRender() 
{
	num_limits = 4;

	BodySize LimitsSize[4] = { {2, 130, 800}, {2, 130, 800}, {800, 130 , 2}, {800, 130, 2} };
	vec3 LimitsPosition[4] = { {-400, 15, 0}, {400, 15, 0}, {0, 15, 400}, {0, 15, -400} };

	for (int j = 0; j < num_limits; j++)
	{
		LimitCubes.PushBack(new Cube(LimitsSize[j].sizeX, LimitsSize[j].sizeY, LimitsSize[j].sizeZ));
		LimitCubes[j]->SetPos(LimitsPosition[j].x, LimitsPosition[j].y, LimitsPosition[j].z);
		//LimitCubes[j]->color = Black;
		bodyLimitCubes.PushBack(App->physics->AddBody(*(LimitCubes[j]), 0.0f));
	}
}

void ModuleSceneIntro::RoadRender()
{
	num_roads = 95;

	BodySize curva_oberta = { 2,5,50 };
	BodySize curva_tancada = { 2,5,10 };

	//						  //   	recta 1      //          curva 1          //     recta 2      //             pont 1              //      recta 3     //         curva 2           //
	BodySize RoadSize[95] = { {2,5,100}, {2,5,100}, curva_oberta, curva_tancada, {2,5,15}, {2,5,15}, {15,2,23}, {32,2,23}, {15,2,23}, {2,5,100}, {2,5,100}, curva_oberta, curva_tancada };



	//                        //      recta 1         //         curva 1        //          recta 2         //                       pont 1                      //            recta 3        //         curva 2          //
	vec3 RoadPosition[95] = { {390,2.5,0}, {370,2.5,0}, {373,2.5,66}, {366,2.5,53}, {356,2.5,79}, {356,2.5,56}, {347.5,2.5,67.5}, {325,6.5,67.5}, {302.5,2.5,67.5}, {252,2.5,78}, {252,2.5,58}, {185,2.5,60}, {200,2.5,55} };



	//                               //           recta 1      //             curva 1          //             recta 2           //                pont 1                  //             recta 3          //            curva 2          //
	BodyRotation RoadRotation[95] = { {0, {0,1,0}}, {0, {0,1,0}}, {-45, {0,1,0}}, {-45, {0,1,0}}, {-90, {0,1,0}}, {-90, {0,1,0}}, {-30, {0,0,1}}, {0, {0,1,0}}, {30,{0,0,1}}, {-90,{0,1,0}}, {-90,{0,1,0}}, {-135, {0,1,0}}, {-135,{0,1,0}} };




	for (int i = 0; i < num_roads; i++) 
	{
		RoadCubes.PushBack(new Cube(RoadSize[i].sizeX, RoadSize[i].sizeY, RoadSize[i].sizeZ));
		RoadCubes[i]->SetPos(RoadPosition[i].x, RoadPosition[i].y, RoadPosition[i].z);
		RoadCubes[i]->SetRotation(RoadRotation[i].angle, RoadRotation[i].axis_pos);
		RoadCubes[i]->color = Brown;
		bodyRoadCubes.PushBack(App->physics->AddBody(*(RoadCubes[i]), 0.0f));
	}
}

void ModuleSceneIntro::DetailsRender() 
{

}

void ModuleSceneIntro::PeopleRender() 
{

}
void ModuleSceneIntro::BuildingsRender() 
{

}