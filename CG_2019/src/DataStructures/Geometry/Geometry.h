#pragma once

//
// Base class for geometry
//

#include <vector>

#include "Ray.h"

class Geometry
{
public:
	Geometry();
	~Geometry();

	virtual CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray) = 0;//return all the colisions of a set of rays
	virtual CRAB::RayCollisionList Collide(const CRAB::Ray &ray) = 0;//return the all the ray collisions
};

