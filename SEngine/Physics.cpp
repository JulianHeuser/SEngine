#include "Physics.h"

Physics::Physics()
{
	dInitODE2(dInitFlagManualThreadCleanup);
	dAllocateODEDataForThread((unsigned int)dAllocateMaskAll);
	

	world = dWorldCreate();
	space = dSimpleSpaceCreate(0);
	contactGroup = dJointGroupCreate(0);


	dWorldSetERP(world, 0.2);
	dWorldSetCFM(world, 1e-5);
	
	dWorldSetContactMaxCorrectingVel(world, 0.9);
	dWorldSetContactSurfaceLayer(world, 0.001);

	dWorldSetGravity(world, 0, -8, 0);
}
