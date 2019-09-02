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


float Triangle::CollideClosest(register const CRAB::Ray &ray) const {
	
	const Vector4Df &t = dot_simd_Vec(v1 - ray.origin, normal) / dot_simd_Vec(ray.direction, normal);
	
	const Vector4Df &p_plane = ray.origin + ray.direction*t;
	
	const float proj1 = dot_simd(p_plane - v1, n_e1);
	const float proj2 = dot_simd(p_plane - v1, n_e2);
	
	if (proj1 >= 0.000001f && proj2 >= 0.000001f && proj2 + proj1 <= normal.w) {
		return _mm_cvtss_f32(t);
	}
	return INFINITY;

}

CRAB::RayCollisionList Triangle::Collide(const CRAB::Ray &ray)
{
	//Colisions
	CRAB::RayCollisionList col;


	return col;

}