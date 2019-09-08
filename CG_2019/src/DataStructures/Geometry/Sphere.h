#pragma once
#include "Geometry.h"
class Sphere : public Geometry
{
	
public:
	CRAB::Vector4Df center; // Sphere center
	float radius;			// Sphere radius
	
	Sphere();
	Sphere(const CRAB::Vector4Df &C, float R);
	~Sphere();

	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray);
	float CollideClosest(const CRAB::Ray &ray) const;
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray);
};

