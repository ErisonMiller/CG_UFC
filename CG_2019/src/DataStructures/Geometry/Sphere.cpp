#include <iostream>
#include "Sphere.h"

using namespace CRAB;

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
	CRAB::RayCollisionList col, col_aux;

	for (int i = 0; i < ray.size(); i++)
	{
		col_aux = this->Collide(ray.at(i));
		col.collisions.insert(col.collisions.end(), col_aux.collisions.begin(), col_aux.collisions.end());
	}

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
	CRAB::Collision t;

	t.geometry = this;

	CRAB::Vector4Df d = ray.direction.to_unitary();	// Vector that defines the Ray direction
	CRAB::Vector4Df W = ray.origin - center;		// The difference of P0 - C
	
	// Coefficients of the equation
	float A = dot(d, d);
	float B = 2 * dot(W, d);
	float C = dot(W, W) - (radius * radius);
	// Discriminant
	float Delta = (B*B) - (4*A*C);

	if (Delta == 0.0) // One intersection
	{
		t.distance = (-B) / (2 * A);
		t.pint = ray.origin + (d * t.distance);
		col.collisions.push_back(t);
	}
	else if (Delta > 0.0) // Two intersections
	{
		// First point
		t.distance = (-B - sqrtf(Delta)) / (2 * A);
		t.pint = ray.origin + (d * t.distance);
		col.collisions.push_back(t);
		// Second point
		t.distance = (-B + sqrtf(Delta)) / (2 * A);
		t.pint = ray.origin + (d * t.distance);
		col.collisions.push_back(t);
	}

	return col;
}