#include "Cube.h"

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
	quads[1] = Quad(p4, p5, p6, p7);

	quads[2] = Quad(p0, p1, p5, p4);
	quads[3] = Quad(p2, p3, p7, p6);

	quads[4] = Quad(p1, p2, p6, p5);
	quads[5] = Quad(p3, p0, p4, p7);

	center = base + _up * (size*0.5f);
	const float size2 = size * size;
	r2 = size2 * 0.5f + size2 * 0.25f;
}

Cube::~Cube()
{
}



CRAB::RayCollisionList Cube::CollideAll(const std::vector<CRAB::Ray> &ray)
{
	//Colisions
	CRAB::RayCollisionList col;

	return col;
}


__forceinline float Cube::CollideClosest(register const CRAB::Ray &ray) const {
	//Test if the ray collides with the sphere that sorrounds the cube 
	const Vector4Df &n = cross_simd(center - ray.origin, ray.direction);
	if (dot_simd(n, n) > r2)return INFINITY;//don't collide
	

	float final_distance = INFINITY;

	for (unsigned i = 0; i < 6; i++) {
		const float dist = quads[i].CollideClosest(ray);
		
		if (dist < final_distance)final_distance = dist;
	}
	return final_distance;
}

CRAB::RayCollisionList Cube::Collide(const CRAB::Ray &ray)
{
	//Colisions
	CRAB::RayCollisionList col;

	for (int i = 0; i < 6; i++) {
		RayCollisionList col_quad = quads[i].Collide(ray);
		col.collisions.insert(col.collisions.end(), col_quad.collisions.begin(), col_quad.collisions.end());
	}
	
	return col;

}