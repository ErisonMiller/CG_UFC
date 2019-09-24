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
	
public:
	float height, radius;
	CRAB::Vector4Df base_center; // Cylinder base center
	CRAB::Vector4Df direction;  // Cylinder direction vector
	
	Cylinder();
	Cylinder(float height, float radius, CRAB::Vector4Df base_center, CRAB::Vector4Df direction);
	~Cylinder();

	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray);
	CRAB::Collision CollideClosest(const CRAB::Ray &ray);
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray); 

	CRAB::Vector4Df getNormal(const CRAB::Vector4Df &point);

	void transform(CRAB::Matrix4 m);
};

