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
	CRAB::Collision CollideClosest(const CRAB::Ray &ray);
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray);

	CRAB::Vector4Df getNormal(const CRAB::Vector4Df &point);

	void transform(CRAB::Matrix4 m);
};

