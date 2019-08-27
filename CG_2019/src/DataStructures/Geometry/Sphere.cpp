#include "Sphere.h"

Sphere::Sphere()
{

}

Sphere::Sphere(const CRAB::Vector4Df &C, float &R)
{
	center = C;
	radius = R;
}

Sphere::~Sphere()
{

}

//TODO: Implement it
CRAB::RayCollisionList Sphere::CollideAll(const std::vector<CRAB::Ray> &ray)
{
	//Colisions
	CRAB::RayCollisionList col;

	return col;
}

//TODO: Implement it
CRAB::RayCollisionList Sphere::CollideClosest(const CRAB::Ray &ray)
{
	CRAB::RayCollisionList col;

	return col;
}

//TODO: Implement it
CRAB::RayCollisionList Sphere::Collide(const CRAB::Ray &ray)
{
	//Colisions
	CRAB::RayCollisionList col;

	return col;
}