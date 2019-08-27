//TODO: - Collision:
//			- Handle a == 0;
//			- Handle Base Intersection

#include "Cone.h"

Cone::Cone()
{

}

Cone::Cone(float height, float radius, CRAB::Vector4Df base_center, CRAB::Vector4Df direction)
{
	this->height = height;
	this->radius = radius;
	this->base_center = base_center;
	this->direction = direction;
	this->top_vertex = this->base_center + (this->direction * this->height);
}

Cone::~Cone()
{

}

CRAB::RayCollisionList Cone::CollideAll(const std::vector<CRAB::Ray> &ray)
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
CRAB::RayCollisionList Cone::CollideClosest(const CRAB::Ray &ray) {
	CRAB::RayCollisionList col;

	return col;
}

CRAB::RayCollisionList Cone::Collide(const CRAB::Ray &ray)
{
	//Colisions
	CRAB::RayCollisionList col;
	CRAB::Collision t;

	t.geometry = this;

	//Aux Variables
	float h_2 = this->height * this->height;
	float r_2 = this->radius * this->radius;
	float cos_alfa_2 = (h_2 / (h_2 + r_2));

	float d_dot_n = dot(ray.direction, this->direction);
	float d_dot_d = dot(ray.direction, ray.direction);

	CRAB::Vector4Df v = this->top_vertex - ray.origin; // Vector between the Cone top vertex and the ray origin.

	float v_dot_n = dot(v, this->direction);

	//Equantion Constants
	float a = (d_dot_n*d_dot_n) - (d_dot_d * cos_alfa_2);
	float b = 2 * ((dot(v, ray.direction)*cos_alfa_2) - (v_dot_n * d_dot_n));
	float c = (v_dot_n*v_dot_n) - (dot(v, v)*cos_alfa_2);
	//Delta
	float delta = (b*b) - (4 * a*c);

	if (a != 0) {

		if (delta == 0) { // One intersection
			t.distance = (-b / (2 * a));
			CRAB::Vector4Df p = ray.origin + (ray.direction * t.distance); // Intersection Point
			float p_projection = dot((this->top_vertex - p), this->direction); //Projection of the point P on the cone axis
			if (p_projection >= 0 && p_projection <= this->height) { // Does the ray hit the cone?
				t.pint = p;
				col.collisions.push_back(t);
			}
		}
		else if (delta > 0) {
			delta = sqrtf(delta);
			//First Point
			t.distance = ((-1)*(delta + b)) / (2 * a);
			CRAB::Vector4Df p = ray.origin + (ray.direction * t.distance); // Intersection Point
			float p_projection = dot((this->top_vertex - p), this->direction); //Projection of the point P on the cone axis
			if (p_projection >= 0 && p_projection <= this->height) { // Does the ray hit the cone?
				t.pint = p;
				col.collisions.push_back(t);
			}

			//Second Point
			t.distance = (delta - b) / (2 * a);
			p = ray.origin + (ray.direction * t.distance); // Intersection Point
			p_projection = dot((this->top_vertex - p), this->direction); //Projection of the point P on the cone axis
			if (p_projection >= 0 && p_projection <= this->height) { // Does the ray hit the cone?
				t.pint = p;
				col.collisions.push_back(t);
			}
		}
	}

	//TODO: It must be implmented only if one of the cone interception is not valid or 'a' is less than 0.
	//Base
	CRAB::Vector4Df base_dir = this->direction * (-1);
	//OBS.: Base Center is a point on the base plan.
	t.distance = dot((this->base_center - ray.origin), base_dir) / (dot(ray.direction, base_dir));
	CRAB::Vector4Df p = ray.origin + (ray.direction * t.distance); // Intersection Point
	float int_to_center = (p - this->base_center).length(); // Distance of the intersection point from the base center.
	
	if (int_to_center <= this->radius) {//The point intercept tha base iff its distance from the center is less than the radius.
		t.pint = p;
		col.collisions.push_back(t);
	}

	return col;
}