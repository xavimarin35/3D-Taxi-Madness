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
	num_roads = 62;

	BodySize curva_oberta = { 2,5,50 };
	BodySize curva_tancada = { 2,5,10 };

	//						  //   	recta 1      //          curva 1          //     recta 2      //             pont 1              //      recta 3     //         curva 2           //        recta 4     //           curva 3         //        recta 5     //          curva 4          //        recta 6     //          curva 5       //        recta 7     //         curva 6          //         recta 8    //         curva 7           //              pont 2          //        recta 9     //        recta 10   //        recta 11    //        recta 13    //      recta 12    //         curva 11          //       recta 14     //   recta auxiliar   //          curva 12        //        recta 15  //        recta 16  //  baixada  //  recta  //  recta 2 //  pujada //
	BodySize RoadSize[62] = { {2,5,100}, {2,5,100}, curva_oberta, curva_tancada, {2,5,15}, {2,5,15}, {15,2,23}, {32,2,23}, {15,2,23}, {2,5,100}, {2,5,100}, curva_oberta, curva_tancada, {2,5,150}, {2,5,143}, curva_oberta, curva_tancada, {2,5,400}, {2,5,422}, curva_oberta, curva_tancada, {2,5,400}, {2,5,460}, {2,5,50}, curva_tancada, {2,5,100}, {2,5,100}, curva_oberta, curva_tancada, {2,5,175}, {2,5,175}, curva_oberta, curva_tancada, {15,2,23}, {32,2,23}, {15,2,23}, {2,5,390}, {2,5,390}, {2,5,50}, {2,5,25}, {2,5,275}, {2,5,200}, {2,5,600}, {2,5,600}, {2,5,90}, {2,5,40}, curva_oberta, curva_tancada, {2,5,300}, {2,5,325}, {2,5,200}, {2,5,200}, curva_oberta, curva_tancada, {2,5,35}, {2,5,20}, {2,5,40}, {2,5,60}, {15,23,2}, {15,2,200}, {15,2,170}, {350,2,50} };

	//                        //      recta 1         //         curva 1        //          recta 2         //                       pont 1                      //            recta 3        //         curva 2          //            recta 4         //            curva 3            //           recta 5           //             curva 4             //            recta 6            //            curva 5            //           recta 7            //              curva 6          //             recta 8          //            curva 7            //                       pont 2                     //              recta 9        //            recta 10      //            recta 11         //          recta 13         //            recta 12        //              curva 11       //             recta 14         //            recta auxiliar     //               curva 12        //              recta 15           //             recta 16          //   baixada  //       recta  //     recta 2   //    pujada  //
	vec3 RoadPosition[62] = { {390,2.5,0}, {370,2.5,0}, {373,2.5,66}, {366,2.5,53}, {356,2.5,79}, {356,2.5,56}, {347.5,2.5,67.5}, {325,6.5,67.5}, {302.5,2.5,67.5}, {252,2.5,78}, {252,2.5,58}, {185,2.5,60}, {200,2.5,55}, {197,2.5,-23}, {167,2.5,-22.5}, {180,2.5,-115}, {164,2.5,-97.5}, {-38,2.5,-100}, {-40,2.5,-130}, {-270,2.5,-113}, {-241,2.5,-98}, {-243.5,2.5,105}, {-275,2.5,120}, {-258,2.5,340}, {-240,2.5,308}, {-187,2.5,311}, {-187,2.5,340}, {-120,2.5,323}, {-133,2.5,307.5}, {-100,2.5,217}, {-130,2.5,217}, {-113,2.5,111}, {-97,2.5,128}, {347.5,2.5,107.5}, {325,6.5,107.5}, {302.5,2.5,107.5}, {100,2.5,98}, {100,2.5,118}, {350,2.5,98}, {340,2.5,118}, {375,2.5,220}, {350,2.5,220}, {310,2.5,10}, {325,2.5,-10}, {347,2.5,337}, {337,2.5,305}, {308,2.5,-323}, {307,2.5,-292}, {155,2.5,-293}, {150,2.5,-320}, {-70,2.5,-292}, {-70,2.5,-320}, {-190,2.5,-302}, {-172,2.5,-291}, {-197,2.5,-275}, {-176,2.5,-280}, {-155,2.5,-270}, {-170,2.5,-256}, {380,6,-50}, {380,11.5,-159}, {295,11.5,-252}, {60,6,-250} };

	//                               //           recta 1      //             curva 1          //             recta 2           //                pont 1                  //             recta 3          //            curva 2          //          recta 4        //             curva 3           //              recta 5       //            curva 4           //          recta 6        //              curva 5          //             recta 7         //           curva 6          //          recta 8        //             curva 7          //                     pont 2               //           recta 9          //             recta 10           //          recta 11      //          recta 13       //            recta 12           //             curva 11          //            recta 14         //         recta auxiliar      //            curva 12          //          recta 15      //         recta 16          //     baixada  //      recta  //   recta 2  //   pujada  //
	BodyRotation RoadRotation[62] = { {0, {0,1,0}}, {0, {0,1,0}}, {-45, {0,1,0}}, {-45, {0,1,0}}, {-90, {0,1,0}}, {-90, {0,1,0}}, {-30, {0,0,1}}, {0, {0,1,0}}, {30,{0,0,1}}, {-90,{0,1,0}}, {-90,{0,1,0}}, {-135, {0,1,0}}, {-135,{0,1,0}}, {0, {0,1,0}}, {0,{0,1,0}}, {-135, {0,1,0}}, {-135,{0,1,0}}, {-90,{0,1,0}}, {-90,{0,1,0}}, {-45, {0,1,0}}, {-45, {0,1,0}}, {0, {0,1,0}}, {0,{0,1,0}}, {-90, {0,1,0}}, {-135,{0,1,0}}, {-90,{0,1,0}}, {-90,{0,1,0}}, {-45, {0,1,0}}, {-45, {0,1,0}}, {0, {0,1,0}}, {0,{0,1,0}}, {-45, {0,1,0}}, {-45, {0,1,0}}, {-30, {0,0,1}}, {0, {0,1,0}}, {30,{0,0,1}}, {-90,{0,1,0}}, {-89.5,{0,1,0}}, {-90,{0,1,0}}, {-90,{0,1,0}}, {0, {0,1,0}}, {0,{0,1,0}}, {0, {0,1,0}}, {0,{0,1,0}}, {-125, {0,1,0}}, {-140, {0,1,0}}, {-135, {0,1,0}}, {-135,{0,1,0}}, {-89.5,{0,1,0}}, {-90,{0,1,0}}, {-90,{0,1,0}}, {-90,{0,1,0}}, {-45, {0,1,0}}, {-45, {0,1,0}}, {0,{0,1,0}}, {0,{0,1,0}}, {-90,{0,1,0}}, {-90,{0,1,0}}, {-60,{1,0,0}}, {0, {0,1,0}}, {-90,{0,1,0}}, {2,{0,0,1}} };

	for (int i = 0; i < num_roads; i++) 
	{
		RoadCubes.PushBack(new Cube(RoadSize[i].sizeX, RoadSize[i].sizeY, RoadSize[i].sizeZ));
		RoadCubes[i]->SetPos(RoadPosition[i].x, RoadPosition[i].y, RoadPosition[i].z);
		RoadCubes[i]->SetRotation(RoadRotation[i].angle, RoadRotation[i].axis_pos);
		RoadCubes[i]->color = Blue2;		
		bodyRoadCubes.PushBack(App->physics->AddBody(*(RoadCubes[i]), 0.0f));
	}
}

void ModuleSceneIntro::DetailsRender() 
{
	num_details = 10;

	BodySize pas_zebra = { 4,2,20 };
	BodyRotation no_rotation = { 0,{0,1,0} };

	BodySize DetailsSize[10] = { {2,10,2}, {2,10,2}, {18,3.3,2}, pas_zebra, pas_zebra, pas_zebra, pas_zebra, pas_zebra };
	vec3 DetailsPosition[10] = { {390,10,15}, {370,10,15}, {380,13,15}, {182,-0.4,-23}, {188,-0.4,-23}, {176,-0.4,-23}, {170,-0.4,-23}, {194,-0.4,-23} };
	BodyRotation DetailsRotation[10] = { no_rotation, no_rotation, no_rotation, no_rotation, no_rotation, no_rotation, no_rotation, no_rotation };

	for (int i = 0; i < num_details; i++) 
	{
		DetailsCubes.PushBack(new Cube(DetailsSize[i].sizeX, DetailsSize[i].sizeY, DetailsSize[i].sizeZ));
		DetailsCubes[i]->SetPos(DetailsPosition[i].x, DetailsPosition[i].y, DetailsPosition[i].z);
		DetailsCubes[i]->SetRotation(DetailsRotation[i].angle, DetailsRotation[i].axis_pos);
		if (i == 0 || i == 1) 
		{
			DetailsCubes[i]->color = Black;
		}
		else if (i == 2) 
		{
			DetailsCubes[i]->color = Yellow;
		}
		else if (i > 2 && i < 8) 
		{
			DetailsCubes[i]->color = White;
		}
		bodyDetailsCubes.PushBack(App->physics->AddBody(*(DetailsCubes[i]), 0.0f));
	}
}

void ModuleSceneIntro::PeopleRender() 
{
	num_people = 51;
	BodyRotation no_rotation = { 0, {0,1,0} };
	BodySize face = { 1,1,1 };
	BodySize body = { 1.5,2.5,1 };

	BodySize PeopleSize[51] = { {}, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body, face, body };
	vec3 PeoplePosition[51] = { {}, {180,4.2,-23}, {180,2.5,-23} };
	BodyRotation PeopleRotation[51] = { {}, no_rotation, no_rotation };

	for (int i = 0; i < num_people; i++) 
	{
		PeopleCubes.PushBack(new Cube(PeopleSize[i].sizeX, PeopleSize[i].sizeY, PeopleSize[i].sizeZ));
		PeopleCubes[i]->SetPos(PeoplePosition[i].x, PeoplePosition[i].y, PeoplePosition[i].z);
		PeopleCubes[i]->SetRotation(PeopleRotation[i].angle, PeopleRotation[i].axis_pos);
		bodyPeopleCubes.PushBack(App->physics->AddBody(*(PeopleCubes[i]), 0.0f));
		
		if (i % 2 == 0)
		{
			if (i > 0 && i < 12 || i > 26 && i < 38) 
			{
				PeopleCubes[i]->color = Green;
			}

			else PeopleCubes[i]->color = Red;
		}
		
		else
		{
			if (i > 0 && i < 12 || i > 38 && i <= 50) 
			{
				PeopleCubes[i]->color = Skin2;
			}

			else PeopleCubes[i]->color = Skin;
		}
	}
}

void ModuleSceneIntro::BuildingsRender() 
{

}