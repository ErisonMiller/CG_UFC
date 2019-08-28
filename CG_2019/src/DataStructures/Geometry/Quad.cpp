#include "Quad.h"

using namespace CRAB;

Quad::Quad()
{
}

Quad::Quad(const CRAB::Vector4Df &_v1, const CRAB::Vector4Df &_v2, const CRAB::Vector4Df &_v3, const CRAB::Vector4Df &_v4) :
	v1(_v1) {
	e1 = _v2 - _v1;
	e2 = _v4 - _v1;
	size1 = e1.length();
	size2 = e2.length();
	e1.normalize();
	e2.normalize();
}

Quad::~Quad()
{
}


CRAB::RayCollisionList Quad::CollideAll(const std::vector<CRAB::Ray> &ray)
{
	//Colisions
	CRAB::RayCollisionList col;

	return col;
}

//TODO: Implement it
float Quad::CollideClosest(register const CRAB::Ray &ray) {
	
	Vector4Df normal;
	normal.v128 = cross_simd(e1, e2);

	const Vector4Df &t = dot_simd_Vec(v1 - ray.origin, normal) / dot_simd_Vec(ray.direction, normal);

	const Vector4Df &p_plane = ray.origin + ray.direction*t;


	//const Vector4Df &proj1 = dot_simd_Vec(p_plane - v1, e1);
	//const Vector4Df &proj2 = dot_simd_Vec(p_plane - v1, e2);


	//const Vector4Df tst{
	//	_mm_cvtss_f32(proj1),
	//	_mm_cvtss_f32(proj2),
	//	size1,
	//	size2
	//};
	//const Vector4Df tst2{
	//	0.0f,
	//	0.0f,
	//	_mm_cvtss_f32(proj1),
	//	_mm_cvtss_f32(proj2)
	//};

	//if (_mm_test_all_ones(_mm_castps_si128(_mm_cmpgt_ps(tst, tst2)))) {
	//	return t.x;
	//}
	
	const float proj1 = dot_simd(p_plane - v1, e1);
	const float proj2 = dot_simd(p_plane - v1, e2);

	if (proj1 >= 0.000001f && proj1 <= size1 && proj2 >= 0.000001f && proj2 <= size2) {
		return _mm_cvtss_f32(t);
	}
	return INFINITY;
}

CRAB::RayCollisionList Quad::Collide(const CRAB::Ray &ray)
{
	//Colisions
	CRAB::RayCollisionList col;
	

	const Vector4Df normal = cross(e1, e2);

	float t = dot(v1 - ray.origin, normal) / dot(ray.direction, normal);

	const Vector4Df p_plane = ray.origin + ray.direction*t;

	float proj1 = dot(p_plane - v1, e1);
	float proj2 = dot(p_plane - v1, e2);

	if (proj1 >= 0.000001f && proj1 <= size1 && proj2 >= 0.000001f && proj2 <= size2) {
		col.collisions.push_back(CRAB::Collision{t, p_plane, this});
	}

	return col;

}