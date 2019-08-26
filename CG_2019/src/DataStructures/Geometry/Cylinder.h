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
	virtual ~Cylinder() = 0;

	CRAB::RayCollisionList CollideAll(const CRAB::Ray &ray);//return all the colisions
	float Collide(const CRAB::Ray &ray);//return the closest collision 
};

