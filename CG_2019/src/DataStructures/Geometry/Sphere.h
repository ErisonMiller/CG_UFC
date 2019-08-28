#pragma once
#include "Geometry.h"
class Sphere : public Geometry
{
	CRAB::Vector4Df center; // Sphere center
	float radius;			// Sphere radius

public:
	Sphere();
	Sphere(const CRAB::Vector4Df &C, float &R);
	~Sphere();

	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray);
	float CollideClosest(const CRAB::Ray &ray);
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray);
};

