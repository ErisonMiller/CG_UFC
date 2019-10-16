#include "Cube.h"
#include <iostream>

using namespace CRAB;

Cube::Cube()
{
}


Cube::Cube(const CRAB::Vector4Df &base, const CRAB::Vector4Df &up, const CRAB::Vector4Df &front, float size)
{
	const Vector4Df _up = up.to_unitary();
	const Vector4Df _front = front.to_unitary();
	const Vector4Df _left = cross(_up, _front);

	const Vector4Df p0 = base + (_front * size * 0.5f) + (_left * size * 0.5f);
	const Vector4Df p1 = p0 - (_left * size);
	const Vector4Df p2 = p1 - (_front * size);
	const Vector4Df p3 = p2 + (_left * size);

	const Vector4Df p4 = 	 p0 + (_up*size);
	const Vector4Df p5 = 	 p1 + (_up*size);
	const Vector4Df p6 = 	 p2 + (_up*size);
	const Vector4Df p7 = 	 p3 + (_up*size);

	quads[0] = Quad(p0, p1, p2, p3);
	quads[1] = Quad(p4, p7, p6, p5);

	quads[2] = Quad(p0, p4, p5, p1);
	quads[3] = Quad(p2, p6, p7, p3);

	quads[4] = Quad(p1, p5, p6, p2);
	quads[5] = Quad(p3, p7, p4, p0);

	center = base + _up * (size*0.5f);
	const float size2 = size * size;
	r2 = size2 * 0.5f + size2 * 0.25f;
}

Cube::~Cube()
{
}

__forceinline float CollideClosestQuad(register const CRAB::Ray &ray, register const Quad &quad) {

	//const Vector4Df &normal = cross_simd(_mm_castsi128_ps(_mm_stream_load_si128((__m128i*)&(quad.e1))),
	//	_mm_castsi128_ps(_mm_stream_load_si128((__m128i*)&(quad.e2))));
	const Vector4Df &normal = cross_simd(quad.e1, quad.e2);


	const Vector4Df &dir = dot_simd_Vec(ray.direction, normal);
	if (_mm_cvtss_f32(dir) > 0.0f)return INFINITY;
	const Vector4Df &t = dot_simd_Vec(quad.v1 - ray.origin, normal) / dir;

	const Vector4Df &p_plane = ray.origin + ray.direction*t;
	//const CRAB::Vector4Df &p_plane = *(Vector4Df*)&_mm_fmadd_ps(ray.direction, t, ray.origin);


	const float proj2 = dot_simd(p_plane - quad.v1, quad.e2);
	const float proj1 = dot_simd(p_plane - quad.v1, quad.e1);

	//const bool hit = proj1 >= 0.000001f && proj1 <= quad.size1 && proj2 >= 0.000001f && proj2 <= quad.size2;
	//if (hit) {
	if(proj1 >= 0.000001f && proj2 >= 0.000001f && proj1 <= quad.size1 && proj2 <= quad.size2){
	//if (proj1 >= 0.000001f && proj2 >= 0.000001f && proj2 + proj1 <= (quad.size2 + quad.size1)*0.5f) {//to be a triangle
		return _mm_cvtss_f32(t);
	}
	return INFINITY;
}

__forceinline float CollideClosestQuad2(register const __m256 &o, register const __m256 &d, register const Quad &quad, register const Quad &quad2) {


	const __m256 &normal = _mm256_set_m128(cross_simd(quad.e1, quad.e2), cross_simd(quad2.e1, quad2.e2));

	const __m256 &v1 = _mm256_set_m128(quad.v1, quad2.v1);
	
	const __m256 &t = _mm256_div_ps(
		_mm256_dp_ps(_mm256_sub_ps(v1, o), normal, 0xff),
		_mm256_dp_ps(d, normal, 0xff)
		);

	const __m256 &p_plane = _mm256_add_ps(o, _mm256_mul_ps(t,d));

	const __m256 &e1 = _mm256_set_m128(quad.e1, quad2.e1);

	const __m256 &e2 = _mm256_set_m128(quad.e2, quad2.e2);


	const __m256 &proj1 = _mm256_dp_ps(_mm256_sub_ps(p_plane, v1), e1, 0xff);
	const __m256 &proj2 = _mm256_dp_ps(_mm256_sub_ps(p_plane, v1), e2, 0xff);
	
	float dist1 = INFINITY, dist2 = INFINITY;
	if (_mm256_cvtss_f32(proj1) >= 0.000001f && _mm256_cvtss_f32(proj1) <= quad.size1 && _mm256_cvtss_f32(proj2) >= 0.000001f && _mm256_cvtss_f32(proj2) <= quad.size2) {
		dist1 = _mm256_cvtss_f32(t);
	}


	if (_mm_cvtss_f32(_mm256_extractf128_ps(proj1, 1)) >= 0.000001f && _mm_cvtss_f32(_mm256_extractf128_ps(proj1, 1)) <= quad.size1
		&& _mm_cvtss_f32(_mm256_extractf128_ps(proj2, 1)) >= 0.000001f && _mm_cvtss_f32(_mm256_extractf128_ps(proj2, 1)) <= quad.size2) {
		dist2 = _mm_cvtss_f32(_mm256_extractf128_ps(t, 1));
	}
	if (dist1 < dist2)return dist1;

	return dist2;
}

CRAB::RayCollisionList Cube::CollideAll(const std::vector<CRAB::Ray> &ray)
{
	//Colisions
	CRAB::RayCollisionList col;

	return col;
}


CRAB::Collision Cube::CollideClosest(register const CRAB::Ray &ray) {
	CRAB::Collision col;
	col.geometry = this;
	
	//Test if the ray collides with the sphere that sorrounds the cube 
	
	const Vector4Df &n = cross_simd(center - ray.origin, ray.direction);
	if (dot_simd(n, n) > r2)col.distance = INFINITY;//don't collide
	
	float final_distance = INFINITY;
	float dist, colide_quad = 0;


	final_distance = CollideClosestQuad(ray, quads[0]);
	dist = CollideClosestQuad(ray, quads[1]); if (dist < final_distance){final_distance = dist;colide_quad = 1;}
	dist = CollideClosestQuad(ray, quads[2]); if (dist < final_distance){final_distance = dist;colide_quad = 2;}
	dist = CollideClosestQuad(ray, quads[3]); if (dist < final_distance){final_distance = dist;colide_quad = 3;}
	dist = CollideClosestQuad(ray, quads[4]); if (dist < final_distance){final_distance = dist;colide_quad = 4;}
	dist = CollideClosestQuad(ray, quads[5]); if (dist < final_distance){final_distance = dist;colide_quad = 5;}
	
	//final_distance = quads[0].CollideClosest(ray);
	//dist = quads[1].CollideClosest(ray); if (dist < final_distance)final_distance = dist;
	//dist = quads[2].CollideClosest(ray); if (dist < final_distance)final_distance = dist;
	//dist = quads[3].CollideClosest(ray); if (dist < final_distance)final_distance = dist;
	//dist = quads[4].CollideClosest(ray); if (dist < final_distance)final_distance = dist;
	//dist = quads[5].CollideClosest(ray); if (dist < final_distance)final_distance = dist;

	col.pint= ray.origin + ray.direction*final_distance;
	col.pint.w = colide_quad;
	col.distance = final_distance;

	return col;
}

CRAB::RayCollisionList Cube::Collide(const CRAB::Ray &ray)
{
	//Colisions
	CRAB::RayCollisionList col;

	const Vector4Df &n = cross_simd(center - ray.origin, ray.direction);
	if (dot_simd(n, n) > r2)return col;//don't collide


	for (int i = 0; i < 6; i++) {
		const RayCollisionList col_quad = quads[i].Collide(ray);
		col.collisions.insert(col.collisions.end(), col_quad.collisions.begin(), col_quad.collisions.end());
	}
	
	return col;

}

CRAB::Vector4Df Cube::getNormal(const CRAB::Vector4Df& point)
{
	//CRAB::Vector4Df n, n_[6], tmp{ 0.0f, 0.0f, 0.0f, 0 };
	//n = tmp;
	//
	//for (int i = 0; i < 6; i++) {
	//	n_[i] = 
	//
	//	if (n_[i] != tmp) 
	//		n = n_[i];
	//}

	return quads[(int)point.w].getNormal(point);
}

void Cube::transform(CRAB::Matrix4 m)
{
	
	for (Quad &quad : quads) {
		quad.transform(m);
	}
	center = m * center;
}
