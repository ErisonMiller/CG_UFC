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

class Geometry;//foward declaration for geometry

namespace CRAB {

	//struct just for readbility
	struct Ray
	{
		Vector4Df origin, direction;
	};

	//Because we to know the object that we collide besides the distance
	__declspec(align(16))//memory aling to speed up acess
	struct Collision
	{
		float distance;
		Vector4Df pint; // Intersection Point
		Geometry *geometry;
	};

	//Since we have a variable size of collisions we create a list of Collisions
	//TODO: in the future this will change for better use of CSG (just the students of Geometric Modeling need to do this part)
	struct RayCollisionList {
		std::vector<Collision> collisions;
	};

}