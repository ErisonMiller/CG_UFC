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

class Cone : public Geometry
{
	float height, radius;
	CRAB::Vector4Df base_center; // Cone base center
	CRAB::Vector4Df direction;   // Cone direction vector
	CRAB::Vector4Df top_vertex;  // Cone top vertex

public:
	Cone();
	Cone(float height, float radius, CRAB::Vector4Df base_center, CRAB::Vector4Df direction);
	~Cone();

	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray);//return all the colisions
	CRAB::RayCollisionList CollideClosest(const CRAB::Ray &ray);
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray);//return the closest collision 
};

