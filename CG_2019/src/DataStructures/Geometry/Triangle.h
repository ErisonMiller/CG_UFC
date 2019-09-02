#pragma once
#include "Geometry.h"
class Triangle :
	public Geometry
{
public:

	union {
		struct { CRAB::Vector4Df v1, n_e1, n_e2, normal; };
		__m256 v[2];
	};

	Triangle(const CRAB::Vector4Df &v1, const CRAB::Vector4Df &v2, const CRAB::Vector4Df &v3);
	Triangle();
	~Triangle();

	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray);
	float CollideClosest(register const CRAB::Ray &ray) const;
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray);
};

