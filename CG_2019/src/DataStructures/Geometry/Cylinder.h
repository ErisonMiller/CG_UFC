#pragma once

//
// Cylinder Class
//

//System Include Files
#include <iostream>

//Project Include Files
#include "Geometry.h"
#include "Linear_Algebra.h"
#include "Ray.h"

class Cylinder : public Geometry
{
	float height, radius;
	CRAB::Vector4Df base_center; // Cylinder base center
	CRAB::Vector4Df direction;  // Cylinder direction vector

public:
	Cylinder();
	Cylinder(float height, float radius, CRAB::Vector4Df base_center, CRAB::Vector4Df direction);
	~Cylinder();

	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray);
	float CollideClosest(const CRAB::Ray &ray) const;
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray); 
};

