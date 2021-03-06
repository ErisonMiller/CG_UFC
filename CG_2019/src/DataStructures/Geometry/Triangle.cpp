#include "Triangle.h"

using namespace CRAB;

Triangle::Triangle()
{
}


Triangle::~Triangle()
{
}

Triangle::Triangle(const Triangle &tri) : v1(tri.v1), n_e1(tri.n_e1), n_e2(tri.n_e2), normal(tri.normal),
	v_n1(tri.v_n1), v_n2(tri.v_n2), v_n3(tri.v_n3)
{
}

Triangle::Triangle(const CRAB::Vector4Df& v1, const CRAB::Vector4Df& v2, const CRAB::Vector4Df& v3) : v1(v1) {
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

Triangle::Triangle(const CRAB::Vector4Df& v1, const CRAB::Vector4Df& v2, const CRAB::Vector4Df& v3,
	const CRAB::Vector4Df _vn1, const CRAB::Vector4Df _vn2, const CRAB::Vector4Df _vn3)
	: v1(v1), v_n1(_vn1), v_n2(_vn2), v_n3(_vn3) {
	
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


CRAB::Collision Triangle::CollideClosest( const CRAB::Ray &ray) {
	
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
	const float proj1 = (dot_simd(point - v1, n_e1));
	const float proj2 = (dot_simd(point - v1, n_e2));

	Vector4Df n = v_n1*(normal.w - (proj1 + proj2)) + v_n2 * proj2 + v_n3 * proj1;
	
	return n.to_unitary();
}

void Triangle::transform(CRAB::Matrix4 m)
{

	float n_size = normal.w;
	normal.w = 0;
	v1 = m * v1;
	Vector4Df e1 = m * cross(n_e1, normal);
	Vector4Df e2 = m * cross(normal, n_e2);



	//n_e1 = cross_simd(normal, e1).to_unitary() * e1_size;
	//n_e2 = cross_simd(e2, normal).to_unitary() * e2_size;

	const Vector4Df n = cross_simd(e1, e2);
	n_size = n.length();
	float e1_size = e1.length();
	float e2_size = e2.length();
	normal = n.to_unitary();
	n_e1 = cross_simd(normal, e1).to_unitary() * e1_size;
	n_e2 = cross_simd(e2, normal).to_unitary() * e2_size;
	normal.w = n_size;
}
