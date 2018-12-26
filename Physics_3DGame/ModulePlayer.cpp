#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	message = "RUN AWAY FROM GANGSTERS!";

	VehicleInfo car;

	// Car properties ----------------------------------------
	
	// Part de sota, groga
	car.chassis_size.Set(2, 0.8, 6);
	car.chassis_offset.Set(0, 1.45, 0);

	// Part de dalt, groga
	car.upper_chassis.Set(2, 0.8, 4);
	car.upper_chassis_offset.Set(0, 2.2, -0.2);

	// Rectangle darrera, negre
	car.upper_chassis2.Set(1.7, 0.6, 0.1);
	car.upper_chassis2_offset.Set(0, 2.2, -2.2);

	// Cartell de dalt, vermell
	car.middle_chassis.Set(1, 0.3, 0.3);
	car.middle_chassis_offset.Set(0, 2.7, 0);

	// Finestres, negres
	car.top_chassis.Set(2.1, 0.5, 2.7);
	car.top_chassis_offset.Set(0, 2.2, -0.2);


	car.mass = 1200.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 5.0f;
	car.suspensionDamping = 10.0f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(INITIAL_POS);

	initialPosMatrix = mat4x4
			(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			380, 2, -10, 0);

	vehicle->SetTransform(initialPosMatrix.M);


	//title values
	laps = 0;
	timer.Stop();
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	delete[] vehicle->info.wheels;
	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{	
	turn = acceleration = brake = 0.0f;

	if (win == false && lose == false)
	{
		//FX motor noise
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			App->audio->PlayFx(App->audio->accelerateFx);
		}

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			acceleration = MAX_ACCELERATION * 2;
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			if (vehicle->GetKmh() > 0.0f) { brake = BRAKE_POWER; }

			if (vehicle->GetKmh() <= 0.0f) { acceleration = -(MAX_ACCELERATION / 2); }
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (checkpoint1 == true && checkpoint2 == false)
			{
				Respawn({ CHECKPOINT1 });
				ChooseMatrix(1);
			}
			else if (checkpoint1 == true && checkpoint2 == true && checkpoint3 == false)
			{
				Respawn({ CHECKPOINT2 });
				ChooseMatrix(2);
			}
			else if (checkpoint1 == true && checkpoint2 == true && checkpoint3 == true && checkpoint4 == false)
			{
				Respawn({ CHECKPOINT3 });
				ChooseMatrix(3);
			}
			else if (checkpoint1 == true && checkpoint2 == true && checkpoint3 == true && checkpoint4 == true && checkpoint5 == false)
			{
				Respawn({ CHECKPOINT4 });
				ChooseMatrix(4);
			}
			else if (checkpoint1 == true && checkpoint2 == true && checkpoint3 == true && checkpoint4 == true && checkpoint5 == true && checkpoint6 == false)
			{
				Respawn({ CHECKPOINT5 });
				ChooseMatrix(5);
			}
			else if (checkpoint1 == true && checkpoint2 == true && checkpoint3 == true && checkpoint4 == true && checkpoint5 == true && checkpoint6 == true && checkpoint7 == false)
			{
				Respawn({ CHECKPOINT6 });
				ChooseMatrix(6);
			}
			else if (checkpoint1 == true && checkpoint2 == true && checkpoint3 == true && checkpoint4 == true && checkpoint5 == true && checkpoint6 == true && checkpoint7 == true)
			{
				Respawn({ CHECKPOINT7 });
				ChooseMatrix(7);
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		App->camera->ChangeCamera(false, false);
		checkpoint1 = checkpoint2 = checkpoint3 = checkpoint4 = checkpoint5 = checkpoint6 = checkpoint7 = false;
		win = false;
		lose = false;
		timer.Start();
		timer.Stop();
		Respawn({ INITIAL_POS });
		vehicle->SetTransform(initialPosMatrix.M);
		laps = 0;
	}

	// Getting the checkpoints
	float CarPosX = vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX();
	float CarPosZ = vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getZ();


	if (CarPosX > 370 && CarPosX < 390 && CarPosZ > 10 && CarPosZ < 11 && checkpoint1 == false) 
	{
		App->audio->PlayFx(App->audio->checkpointFx);
		checkpoint1 = true;
		timer.Start();
		time_started = true;
		laps =+ 1;
		message = "RACE STARTED!";
	}
	else if (CarPosX > 370 && CarPosX < 390 && CarPosZ > 10 && CarPosZ < 11 && checkpoint7 == true && timer.Read() <= 170000) 
	{
		win = true; 
	}
	else if (CarPosX > 370 && CarPosX < 390 && CarPosZ > 10 && CarPosZ < 11 && checkpoint7 == true && timer.Read() > 170000) 
	{
		lose = true;
	}

	if (CarPosX > 165 && CarPosX < 195 && CarPosZ > -48 && CarPosZ < -46 && checkpoint1 == true)
	{
		App->audio->PlayFx(App->audio->checkpointFx);
		checkpoint2 = true;
		if (timer.Read() < 25000) 
		{
			message = "YOU STARTED WELL!";
		}
		else message = "A BAD START, C'MON!";
	}

	if (CarPosX > -202 && CarPosX < -200 && CarPosZ > -129 && CarPosZ < -100 && checkpoint2 == true)
	{
		App->audio->PlayFx(App->audio->checkpointFx);
		checkpoint3 = true;
		if (timer.Read() < 35000)
		{
			message = "CONTINUE LIKE THIS!";
		}
		else message = "GANGSTERS ARE JUST BEHIND YOU!";
	}

	if (CarPosX > -275 && CarPosX < -245 && CarPosZ > 260 && CarPosZ < 261 && checkpoint3 == true)
	{
		App->audio->PlayFx(App->audio->checkpointFx);
		checkpoint4 = true;
		if (timer.Read() < 50000)
		{
			message = "YOU ARE DOING OK, DON'T RELAX!";
		}
		else message = "BAD TIMING, BUT YOU CAN STILL DO IT!";
	}

	if (CarPosX > 262 && CarPosX < 263 && CarPosZ > 100 && CarPosZ < 115 && checkpoint4 == true)
	{
		App->audio->PlayFx(App->audio->checkpointFx);
		checkpoint5 = true;
		if (timer.Read() < 75000)
		{
			message = "YOU HAVE ALMOST DONE IT!";
		}
		else message = "YOU SHOULD ACCELERATE A LITTLE BIT MORE!";
	}

	if (CarPosX > 310 && CarPosX < 323 && CarPosZ > -222 && CarPosZ < -220 && checkpoint5 == true)
	{
		App->audio->PlayFx(App->audio->checkpointFx);
		checkpoint6 = true;
		if (timer.Read() < 115000)
		{
			message = "STAY FOCUSED, THE END IS NEAR!";
		}
		else message = "THE GANGSTERS ARE ABOUT TO CATCH YOU!";
	}

	if (CarPosX > -154 && CarPosX < -152 && CarPosZ > -269 && CarPosZ < -259 && checkpoint6 == true)
	{
		App->audio->PlayFx(App->audio->checkpointFx);
		checkpoint7 = true;
		if (timer.Read() < 140000)
		{
			message = "THE RACE IS ALMOST OVER, YOU'VE DONE IT!";
		}
		else message = "YOU ARE ALMOST DEAD, SAY YOUR LAST WORDS.";
	}

	//win condition
	if (win == true)
	{
		checkpoint1 = false;
		App->camera->ChangeCamera(true, false);
		App->audio->PlayFx(App->audio->winFx, 1);
		message = "CONGRATULATIONS! YOU SCAPED FROM THE GANGSTERS!";
		timer.Stop();
		/*lap_sec = timer.Read() / 1000;
		lap_min = lap_sec / 60;*/
		if (CarPosZ > 40 && CarPosZ < 41)
		{
			vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0, 0, 0 });
			vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0, 0, 0 });
		}
	}

	//lose condition
	if (lose == true)
	{
		App->camera->ChangeCamera(true, false);
		App->audio->PlayFx(App->audio->loseFx, 1);
		message = "YOU WILL RECEIVE NOW A TYPICAL SPANISH 'ENSALADA DE TIBIAS'";
		timer.Stop();
		if (CarPosZ > 40 && CarPosZ < 41)
		{
			vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0, 0, 0 });
			vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0, 0, 0 });
		}
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();
	CameraToPlayer();

	int lap_sec = timer.Read() / 1000;
	int lap_min = lap_sec / 60;
	lap_sec -= lap_min * 60;

	char title[200];
	sprintf_s(title, "%s || Time: %.2i:%.2i", message, lap_min, lap_sec);
	App->window->SetTitle(title);

	LOG("x = %.1f", CarPosX);
	LOG("z = %.1f\n", CarPosZ);

	return UPDATE_CONTINUE;
}

void ModulePlayer::CameraToPlayer()
{
	newCarPos = vehicle->vehicle->getChassisWorldTransform();
	
	initialCarPos = 
	{		
		newCarPos.getOrigin().getX(),
		newCarPos.getOrigin().getY(), 
		newCarPos.getOrigin().getZ() 
	};

	newCarDirection = 
	{		
		newCarPos.getBasis().getColumn(2).getX(),
		newCarPos.getBasis().getColumn(2).getY(),
		newCarPos.getBasis().getColumn(2).getZ()
	};

	App->camera->Position.x = newCameraPos.x;
	App->camera->Position.y = newCameraPos.y;
	App->camera->Position.z = newCameraPos.z;

	newCameraPos = initialCarPos - 9 * newCarDirection;
	App->camera->Position.y = initialCarPos.y + 6;

}

void ModulePlayer::MySetPos(vec3 newPos)
{
	vehicle->SetPos(newPos.x, newPos.y, newPos.z);
}

void ModulePlayer::Respawn(vec3 respawn_pos) 
{
	MySetPos(respawn_pos);
	vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0,0,0 });
	vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0, 0, 0 });

}

void ModulePlayer::ChooseMatrix(int num) 
{
	if (num == 1) 
	{
		checkpoint1Matrix = mat4x4
			(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			380, 2, 25, 0);

		vehicle->SetTransform(checkpoint1Matrix.M);
	}

	else if (num == 2)
	{
		checkpoint2Matrix = mat4x4
			(1, 0, 180, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			182, 2, -65, 0);

		vehicle->SetTransform(checkpoint2Matrix.M);
	}
	else if (num == 3)
	{
		checkpoint3Matrix = mat4x4
			(1, 0, 4, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-205, 2, -117, 0);

		vehicle->SetTransform(checkpoint3Matrix.M);
	}
	else if (num == 4)
	{
		checkpoint4Matrix = mat4x4
			(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-260, 2, 270, 0);

		vehicle->SetTransform(checkpoint4Matrix.M);
	}
	else if (num == 5)
	{
		checkpoint5Matrix = mat4x4
			(1, 0, 0, 0,
			0, 1, 0, 0,
			4, 0, 1, 0,
			264, 2, 107, 0);

		vehicle->SetTransform(checkpoint5Matrix.M);
	}
	else if (num == 6)
	{
		checkpoint6Matrix = mat4x4
			(1, 0, 180, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			317, 2, -225, 0);

		vehicle->SetTransform(checkpoint6Matrix.M);
	}
	else if (num == 7)
	{
		checkpoint7Matrix = mat4x4
			(1, 0, 0, 0,
			0, 1, 0, 0,
			4, 0, 1, 0,
			-150, 2, -263, 0);

		vehicle->SetTransform(checkpoint7Matrix.M);
	}
}



