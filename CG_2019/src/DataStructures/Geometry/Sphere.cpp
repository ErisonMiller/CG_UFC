#include "Sphere.h"

using namespace CRAB;

Sphere::Sphere()
{

}

Sphere::Sphere(const CRAB::Vector4Df &C, float R) : center(C), radius(R)
{
}

Sphere::~Sphere()
{

}

//TODO: Implement it
CRAB::RayCollisionList Sphere::CollideAll(const std::vector<CRAB::Ray> &ray)
{
	//Colisions
	CRAB::RayCollisionList col, col_aux;

	for (unsigned i = 0; i < ray.size(); i++)
	{
		col_aux = this->Collide(ray.at(i));
		col.collisions.insert(col.collisions.end(), col_aux.collisions.begin(), col_aux.collisions.end());
	}

	return col;
}

//TODO: Get the Intercept Point
CRAB::Collision Sphere::CollideClosest(const CRAB::Ray &ray)
{
	CRAB::Collision col;
	col.geometry = this;

	const CRAB::Vector4Df &W = ray.origin - center;		// The difference of P0 - C

	// Coefficients of the equation
	//const float A = 1.0f;//considering that the direction is already normalized
	const float B = dot_simd(W, ray.direction);
	const float C = dot_simd(W, W) - (radius * radius);
	// Discriminant
	const float Delta = (B*B - C);

	if (Delta < 0.0f) { col.distance = INFINITY; return col; }

	//Distance
	col.distance = -(B + sqrtf(Delta));

	//Intersection Point
	col.pint = ray.origin + (ray.direction * col.distance); // Surface 

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

CRAB::Vector4Df Sphere::getNormal(const CRAB::Vector4Df &point)
{
	Vector4Df n = point - center;
	n.normalize();
	return n;
}

void Sphere::transform(CRAB::Matrix4 m)
{
	center = m * center;
}
