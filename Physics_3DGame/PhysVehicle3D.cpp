#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Grey;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	// Render Chassis
	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());
	chassis.color = Yellow;

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	chassis.Render();

	// Render part 1
	Cube upper1(info.upper_chassis.x, info.upper_chassis.y, info.upper_chassis.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&upper1.transform);
	btQuaternion w = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset1(info.upper_chassis_offset.x, info.upper_chassis_offset.y, info.upper_chassis_offset.z);
	offset1 = offset1.rotate(w.getAxis(), q.getAngle());
	upper1.color = Yellow;

	upper1.transform.M[12] += offset1.getX();
	upper1.transform.M[13] += offset1.getY();
	upper1.transform.M[14] += offset1.getZ();

	upper1.Render();

	// Render part 2
	Cube upper2(info.upper_chassis2.x, info.upper_chassis2.y, info.upper_chassis2.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&upper2.transform);
	btQuaternion e = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset2(info.upper_chassis2_offset.x, info.upper_chassis2_offset.y, info.upper_chassis2_offset.z);
	offset2 = offset2.rotate(e.getAxis(), q.getAngle());
	upper2.color = Black;

	upper2.transform.M[12] += offset2.getX();
	upper2.transform.M[13] += offset2.getY();
	upper2.transform.M[14] += offset2.getZ();

	upper2.Render();

	// Render part 3
	Cube middle(info.middle_chassis.x, info.middle_chassis.y, info.middle_chassis.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&middle.transform);
	btQuaternion r = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset_middle(info.middle_chassis_offset.x, info.middle_chassis_offset.y, info.middle_chassis_offset.z);
	offset_middle = offset_middle.rotate(r.getAxis(), q.getAngle());
	middle.color = Red;

	middle.transform.M[12] += offset_middle.getX();
	middle.transform.M[13] += offset_middle.getY();
	middle.transform.M[14] += offset_middle.getZ();

	middle.Render();

	// Render top
	Cube top(info.top_chassis.x, info.top_chassis.y, info.top_chassis.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&top.transform);
	btQuaternion t = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset_top(info.top_chassis_offset.x, info.top_chassis_offset.y, info.top_chassis_offset.z);
	offset_top = offset_top.rotate(r.getAxis(), q.getAngle());
	top.color = Black;

	top.transform.M[12] += offset_top.getX();
	top.transform.M[13] += offset_top.getY();
	top.transform.M[14] += offset_top.getZ();

	top.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}