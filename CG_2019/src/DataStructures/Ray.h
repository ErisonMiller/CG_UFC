#pragma once
#include "Linear_Algebra.h"
#include <vector>
//
//		Ray Collision Core
//
// This header contains the basic struct for:
//		- Ray
//		- Collison
//		- RayCollisionList
//


namespace CRAB {
	class Geometry;//foward declaration for geometry

	//struct just for readbility
	struct Ray
	{
		Vector4Df origin, direction;
	};

	//Because we to know the object that we collide besides the distance
	struct Collision
	{
		float distance;
		Geometry *geometry;
	};

	//Since we have a variable size of collisions we create a list of Collisions
	//TODO: in the future this will change for better use of CSG (just the students of Geometric Modeling need to do this part)
	struct RayCollisionList {
		std::vector<Collision> collisions;
	};

}