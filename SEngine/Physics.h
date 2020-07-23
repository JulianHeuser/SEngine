#pragma once
#include <ode/ode.h>

class Physics
{
public:
	Physics();


	dWorldID GetWorld() { return world; };
	dSpaceID GetSpace() { return space; };
	dJointGroupID GetContacts() { return contactGroup; };

private:
	dWorldID world;
	dJointGroupID contactGroup;

	//Collision spaces
	dSpaceID space; //Primary space for all physics collisions
};

