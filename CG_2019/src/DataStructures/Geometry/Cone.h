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
	CRAB::Vector4Df base_center; // Cone base center
	CRAB::Vector4Df direction;   // Cone direction vector
	CRAB::Vector4Df top_vertex;  // Cone top vertex
	float height, radius, cos_alfa_2;
	float m; //Distance from the center and the pint normal prolongation auntil the cone axis

public:
	Cone();
	Cone(float height, float radius, CRAB::Vector4Df base_center, CRAB::Vector4Df direction);
	~Cone();

	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray);//return all the colisions
	CRAB::Collision __fastcall CollideClosest(const CRAB::Ray &ray);
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray);//return the closest collision 

	CRAB::Vector4Df getNormal(const CRAB::Vector4Df &point); // returns the normal vector at point on a surface
};

