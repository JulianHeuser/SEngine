#pragma once
#include <ode/ode.h>
#include <iostream>

#define MAX_RAY_CONTACTS 2

void RayCallback(void* Data, dGeomID Geometry1, dGeomID Geometry2)
{
	dReal* HitPosition = (dReal*)Data;

	// Check collisions
	dContact Contacts[MAX_RAY_CONTACTS];
	int Count = dCollide(Geometry1, Geometry2, MAX_RAY_CONTACTS, &Contacts[0].geom, sizeof(dContact));
	for (int i = 0; i < Count; i++)
	{

		// Check depth against current closest hit
		if (Contacts[i].geom.depth < HitPosition[3])
		{
			dCopyVector3(HitPosition, Contacts[i].geom.pos);
			HitPosition[3] = Contacts[i].geom.depth;
		}
	}
}

bool RaycastQuery(dSpaceID Space, const dVector3 Start, dVector3 End)
{

	// Calculate direction
	dVector3 Direction;
	dSubtractVectors3(Direction, End, Start);

	// Get length
	dReal Length = dCalcVectorLength3(Direction);
	dReal InverseLength = dRecip(Length);

	// Normalize
	dScaleVector3(Direction, InverseLength);

	// Create ray
	dGeomID Ray = dCreateRay(0, Length);
	dGeomRaySet(Ray, Start[0], Start[1], Start[2], Direction[0], Direction[1], Direction[2]);

	// Check collisions
	dVector4 HitPosition;
	HitPosition[3] = dInfinity;
	dSpaceCollide2(Ray, (dGeomID)Space, HitPosition, &RayCallback);


	// Cleanup
	dGeomDestroy(Ray);

	// Check for hit
	if (HitPosition[3] != dInfinity)
	{
		dCopyVector3(End, HitPosition);

		return true;
	}

	return false;
}


void BoxCallback(void* Data, dGeomID Geometry1, dGeomID Geometry2)
{
	bool* isCol = (bool*)Data;

	// Check collisions
	dContact Contacts[MAX_RAY_CONTACTS];
	int Count = dCollide(Geometry1, Geometry2, MAX_RAY_CONTACTS, &Contacts[0].geom, sizeof(dContact));
	if (Count > 0)
	{
		*isCol = true;
	}
}

bool BoxCheck(dSpaceID Space, dVector3 position, const float sideLength)
{
	// Create box
	dGeomID Box = dCreateBox(0, sideLength, sideLength, sideLength);
	dGeomSetPosition(Box, position[0], position[1], position[2]);
	// Check collisions
	bool isCol = false;
	dSpaceCollide2(Box, (dGeomID)Space, &isCol, &BoxCallback);


	// Cleanup
	dGeomDestroy(Box);

	// Check for hit
	if (isCol)
	{
		return true;
	}

	return false;
}