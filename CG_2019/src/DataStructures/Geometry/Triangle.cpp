#include "Triangle.h"

using namespace CRAB;

Triangle::Triangle()
{
}


Triangle::~Triangle()
{
}

Triangle::Triangle(const CRAB::Vector4Df &v1, const CRAB::Vector4Df &v2, const CRAB::Vector4Df &v3): v1(v1) {
	const Vector4Df e1 = v2 - v1;
	const Vector4Df e2 = v3 - v1;
	const Vector4Df n = cross_simd(e1, e2);
	float n_size = n.length();
	float e1_size = e1.length();
	float e2_size = e2.length();
	normal = n.to_unitary();
	n_e1 = cross_simd(normal, e1).to_unitary() * e1_size;
	n_e2 = cross_simd(e2, normal).to_unitary() * e2_size;
	normal.w = n_size;

}


CRAB::RayCollisionList Triangle::CollideAll(const std::vector<CRAB::Ray> &ray)
{
	//Colisions
	CRAB::RayCollisionList col;

	return col;
}


CRAB::Collision Triangle::CollideClosest(register const CRAB::Ray &ray) {
	
	CRAB::Collision col;

	col.geometry = this;

	const Vector4Df &t = dot_simd_Vec(v1 - ray.origin, normal) / dot_simd_Vec(ray.direction, normal);
	
	const Vector4Df &p_plane = ray.origin - v1 + ray.direction*t;

	col.pint = p_plane;
	
	const float proj1 = dot_simd(p_plane, n_e1);
	const float proj2 = dot_simd(p_plane, n_e2);
	
	if (proj1 >= 0.000001f && proj2 >= 0.000001f && proj2 + proj1 <= normal.w) {
		col.distance = _mm_cvtss_f32(t);
	}
	else
	{
		col.distance = INFINITY;
	}
	return col;

}

CRAB::RayCollisionList Triangle::Collide(const CRAB::Ray &ray)
{
	//Colisions
	CRAB::RayCollisionList col;


	return col;

}

CRAB::Vector4Df Triangle::getNormal(const CRAB::Vector4Df &point)
{
	Vector4Df n = normal;
	n.w = 0;
	return n;
}

void Triangle::tranform(CRAB::Matrix4 m)
{
}
