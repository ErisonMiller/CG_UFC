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
	this->direction   = direction.to_unitary();
}

Cylinder::~Cylinder()
{

}

CRAB::RayCollisionList Cylinder::CollideAll(const std::vector<CRAB::Ray> &ray)
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

//TODO: Implement it
float Cylinder::CollideClosest(const CRAB::Ray &ray) {
	//Aux Variables
	const CRAB::Vector4Df k = ray.origin - this->base_center; // Vector between the Cylinder base center and the ray origin.
	const float k_dot_u = dot_simd(k, this->direction);
	const float d_dot_u = dot_simd(ray.direction, this->direction); // Dot product between ray direction and cylinder one.

	const CRAB::Vector4Df v = k - ((this->direction)*k_dot_u);
	const CRAB::Vector4Df w = ray.direction - (this->direction*d_dot_u);

	const float r_2 = this->radius * this->radius;
	//Equantion Constants
	const float a = dot_simd(w, w);
	const float b = 2.0f * (dot_simd(v, w));
	const float c = dot_simd(v, v) - (r_2);
	//Delta
	const float delta = (b*b) - (4.0f * a*c);
	float distance_final = INFINITY;
	if (a != 0.0f && delta >= 0.0f) {
		const float distance = (sqrtf(delta) + b) / (-2.0f * a);
		const CRAB::Vector4Df p = ray.origin + (ray.direction * distance); // Intersection Point
		float p_projection = dot_simd((p - this->base_center), this->direction); //Projection of the point P on the cylinder axis
		if (p_projection >= 0.0f && p_projection <= this->height) { // Does the ray hit the cylinder?
			distance_final = distance;
		}
	}

	//TODO: It must be implmented only if one of the cylinder interception is not valid or 'a' is less than 0.
	//Base
	const CRAB::Vector4Df base_dir = this->direction * (-1.0f);
	//OBS.: Base Center is a point on the base plan.
	float distance = dot_simd((this->base_center - ray.origin), base_dir) / (dot_simd(ray.direction, base_dir));
	CRAB::Vector4Df p = ray.origin + (ray.direction * distance); // Intersection Point
	float int_to_center = (p - this->base_center).lengthsq(); // Distance of the intersection point from the base center.

	if (int_to_center <= r_2 && distance < distance_final) {//The point intercept tha base iff its distance from the center is less than the radius.
		distance_final = distance;
	}

	//Top
	CRAB::Vector4Df ppi = this->base_center + this->direction*this->height;
	distance = dot_simd((ppi - ray.origin), this->direction) / (dot_simd(ray.direction, this->direction));
	p = ray.origin + (ray.direction * distance); // Intersection Point
	int_to_center = (p - ppi).lengthsq(); // Distance of the intersection point from the base center.
	if (int_to_center <= r_2 && distance < distance_final) {//The point intercept tha base iff its distance from the center is less than the radius.
		distance_final = distance;
	}

	return distance_final;
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
	if (a != 0){
		if (delta == 0) { // One intersection
			t.distance = (-b / (2 * a));
			CRAB::Vector4Df p = ray.origin + (ray.direction * t.distance); // Intersection Point
			float p_projection = dot((p - this->base_center), this->direction); //Projection of the point P on the cylinder axis
			if (p_projection >= 0 && p_projection <= this->height) { // Does the ray hit the cylinder?
				t.pint = p;
				col.collisions.push_back(t);
			}
		}
		else if (delta > 0) {
			//First Point
			delta = sqrtf(delta);
			t.distance = ((-1)*(delta + b)) / (2 * a);
			CRAB::Vector4Df p = ray.origin + (ray.direction * t.distance); // Intersection Point
			float p_projection = dot((p - this->base_center), this->direction); //Projection of the point P on the cylinder axis
			if (p_projection >= 0 && p_projection <= this->height) { // Does the ray hit the cylinder?
				t.pint = p;
				col.collisions.push_back(t);
			}

			//Second Point
			t.distance = (delta - b) / (2 * a);
			p = ray.origin + (ray.direction * t.distance); // Intersection Point
			p_projection = dot((p - this->base_center), this->direction); //Projection of the point P on the cylinder axis
			if (p_projection >= 0 && p_projection <= this->height) { // Does the ray hit the cylinder?
				t.pint = p;
				col.collisions.push_back(t);
			}

		}
	}

	//TODO: It must be implmented only if one of the cylinder interception is not valid or 'a' is less than 0.
	//Base
	CRAB::Vector4Df base_dir = this->direction * (-1);
	//OBS.: Base Center is a point on the base plan.
	t.distance = dot((this->base_center - ray.origin), base_dir)/(dot(ray.direction, base_dir));
	CRAB::Vector4Df p = ray.origin + (ray.direction * t.distance); // Intersection Point
	float int_to_center = (p - this->base_center).length(); // Distance of the intersection point from the base center.

	if (int_to_center <= this->radius){//The point intercept tha base iff its distance from the center is less than the radius.
		t.pint = p;
		col.collisions.push_back(t);
	}

	//Top
	CRAB::Vector4Df ppi = this->base_center + this->direction*this->height;
	t.distance = dot((ppi - ray.origin), this->direction) / (dot(ray.direction, this->direction));
	p = ray.origin + (ray.direction * t.distance); // Intersection Point
	int_to_center = (p - ppi).length(); // Distance of the intersection point from the base center.
	if (int_to_center <= this->radius) {//The point intercept tha top iff its distance from the center is less than the radius.
		t.pint = p;
		col.collisions.push_back(t);
	}


	return col;

}