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
	dSpaceID space;
	dJointGroupID contactGroup;
};

