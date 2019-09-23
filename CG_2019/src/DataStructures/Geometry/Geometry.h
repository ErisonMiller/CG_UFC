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
	virtual ~Geometry() = 0;

	virtual CRAB::Collision CollideClosest(const CRAB::Ray &ray) = 0;//return the closest collision distance of a ray and the geometry
	virtual CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray) = 0;//return all the colisions of a set of rays
	virtual CRAB::RayCollisionList Collide(const CRAB::Ray &ray) = 0;//return all collisions of a ray

	virtual CRAB::Vector4Df getNormal(const CRAB::Vector4Df &point) = 0; // returns the normal vector at point on a surface

	virtual void tranform(CRAB::Matrix4 m) = 0; // Applies the tranform matrix
};

