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
	virtual CRAB::RayCollisionList CollideClosest(const CRAB::Ray &ray) = 0;//return the closest collision of a ray
	virtual CRAB::RayCollisionList Collide(const CRAB::Ray &ray) = 0;//return all collisions of a ray
};

