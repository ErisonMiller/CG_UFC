//TODO: - Collision:
//			- Handle a == 0;
//			- Handle Base Intersection
#include "Cylinder.h"

Cylinder::Cylinder()
{

}

Cylinder::Cylinder(float height, float radius, CRAB::Vector4Df base_center, CRAB::Vector4Df direction)
{
	this->height      = height;
	this->radius      = radius;
	this->base_center = base_center;
	this->direction   = direction;
}

Cylinder::~Cylinder()
{

}

CRAB::RayCollisionList Cylinder::CollideAll(const std::vector<CRAB::Ray> &ray)
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

CRAB::RayCollisionList Cylinder::Collide(const CRAB::Ray &ray)
{
	//Colisions
	CRAB::RayCollisionList col;
	CRAB::Collision t;

	t.geometry = this;

	//Aux Variables
	CRAB::Vector4Df k = ray.origin - this->base_center; // Vector between the Cylinder base center and the ray origin.
	float k_dot_u = dot(k, this->direction);
	float d_dot_u = dot(ray.direction, this->direction); // Dot product between ray direction and cylinder one.

	CRAB::Vector4Df v = k - ((this->direction)*k_dot_u);
	CRAB::Vector4Df w = ray.direction - (this->direction*d_dot_u);

	//Equantion Constants
	float a = dot(w, w);
	float b = 2 * (dot(v, w));
	float c = dot(v, v) - (this->radius * this->radius);
	//Delta
	float delta = (b*b) - (4*a*c);

	if (delta == 0) { // One intersection
		t.distance = (-b / (2*a));
		CRAB::Vector4Df p = ray.origin + (ray.direction * t.distance); // Intersection Point
		float p_projection = dot((p - this->base_center), this->direction); //Projection of the point P on the cylinder axis
		if (p_projection >= 0 && p_projection <= this->height) { // Does the ray hit the cylinder?
			t.pint = p;
			col.collisions.push_back(t);
		}
	}
	else if (delta > 0) {
		delta = sqrtf(delta);
		//First Point
		t.distance = ((-1)*(delta * b)) / (2*a);
		CRAB::Vector4Df p = ray.origin + (ray.direction * t.distance); // Intersection Point
		float p_projection = dot((p - this->base_center), this->direction); //Projection of the point P on the cylinder axis
		if (p_projection >= 0 && p_projection <= this->height) { // Does the ray hit the cylinder?
			t.pint = p;
			col.collisions.push_back(t);
		}

		//Second Point
		t.distance = (delta - b) / (2*a);
		p = ray.origin + (ray.direction * t.distance); // Intersection Point
		p_projection = dot((p - this->base_center), this->direction); //Projection of the point P on the cylinder axis
		if (p_projection >= 0 && p_projection <= this->height) { // Does the ray hit the cylinder?
			t.pint = p;
			col.collisions.push_back(t);
		}
		
	}

	return col;

}