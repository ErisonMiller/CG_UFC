#pragma once
#include "Geometry.h"
class Triangle :
	public Geometry
{
public:

	union {
		struct { CRAB::Vector4Df v1, n_e1, n_e2, normal; };
		__m256 v256[2];
	};

	Triangle(const CRAB::Vector4Df &v1, const CRAB::Vector4Df &v2, const CRAB::Vector4Df &v3);
	Triangle(const Triangle &tri);
	Triangle();
	~Triangle();

	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray);
	CRAB::Collision CollideClosest(const CRAB::Ray &ray);
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray);

	CRAB::Vector4Df getNormal(const CRAB::Vector4Df &point);

	void transform(CRAB::Matrix4 m);
};

