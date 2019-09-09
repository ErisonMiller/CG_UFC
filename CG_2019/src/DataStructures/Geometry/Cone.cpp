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
	this->direction = direction.to_unitary();
	this->top_vertex = this->base_center + (this->direction * this->height);
}

Cone::~Cone()
{

}

CRAB::RayCollisionList Cone::CollideAll(const std::vector<CRAB::Ray> &ray)
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
CRAB::Collision __fastcall Cone::CollideClosest(const CRAB::Ray &ray) {

	CRAB::Collision col;

	col.geometry = this;

	const float d_dot_n = dot_simd(ray.direction, direction);
	//const float d_dot_d = dot_simd(ray.direction, ray.direction);
	//const float d_dot_d = 1.0f; //changed to recieve 1.0f considering that ray.direction has lenght one

	const CRAB::Vector4Df &v = top_vertex - ray.origin; // Vector between the Cone top vertex and the ray origin.

	const float v_dot_n = dot_simd(v, direction);
	
	//Aux Variables
	const float r_2 = this->radius * this->radius;
	const float h_2 = this->height * this->height;
	const float cos_alfa_2 = (h_2 / (h_2 + r_2));

	//Equantion Constants
	const float a = (d_dot_n*d_dot_n) - (cos_alfa_2);
	const float b = ((dot_simd(v, ray.direction)*cos_alfa_2) - (v_dot_n * d_dot_n));
	const float c = (v_dot_n*v_dot_n) - (dot_simd(v, v)*cos_alfa_2);
	//Delta
	const float delta = (b*b) - (a*c);

	if (a != 0.0f && delta >= 0.0f) {
		const float distance = (sqrtf(delta) - b) / a;
		const CRAB::Vector4Df &p = ray.origin + (ray.direction * distance); // Intersection Point
		const float p_projection = dot_simd((top_vertex - p), direction); //Projection of the point P on the cone axis
		if (p_projection >= 0.0f && p_projection <= this->height) { // Does the ray hit the cone?
			col.pint = p;
			col.distance = distance;
			col.pint.w = d_dot_n * col.distance - v_dot_n;
			return col;
		}
	}
	
	//TODO: It must be implmented only if one of the cone interception is not valid or 'a' is less than 0.
	//Base
	const CRAB::Vector4Df &base_dir = this->direction * (-1.0f);
	//OBS.: Base Center is a point on the base plan.
	const float distance = dot_simd((this->base_center - ray.origin), base_dir) / (dot_simd(ray.direction, base_dir));
	const CRAB::Vector4Df &p = ray.origin + (ray.direction * distance); // Intersection Point
	const float int_to_center = (p - this->base_center).lengthsq(); // Distance of the intersection point from the base center.

	if (int_to_center <= r_2) {//The point intercept tha base iff its distance from the center is less than the radius.
		col.pint = p;
		col.distance = distance;
		return col;
	}

	col.distance = INFINITY;

	return col;
}

CRAB::RayCollisionList Cone::Collide(const CRAB::Ray &ray)
{
	//Colisions
	CRAB::RayCollisionList col;
	CRAB::Collision t;

	t.geometry = this;

	//Aux Variables
	const float r_2 = this->radius * this->radius;
	const float h_2 = this->height * this->height;
	const float cos_alfa_2 = (h_2 / (h_2 + r_2));

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

CRAB::Vector4Df Cone::getNormal(const CRAB::Vector4Df &point)
{
	
	CRAB::Vector4Df p = point;
	p.w = 1.0f;
	CRAB::Vector4Df n = ((p - top_vertex) - direction*point.w).to_unitary();
	
	return n;
}
