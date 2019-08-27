#pragma once
#include "Geometry.h"
class Quad :
	public Geometry
{
	CRAB::Vector4Df v1, e1, e2;
	float size1, size2;
public:
	Quad();

	Quad(const CRAB::Vector4Df &_v1, const CRAB::Vector4Df &_v2, const CRAB::Vector4Df &_v3, const CRAB::Vector4Df &_v4);
	~Quad();

	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray);
	CRAB::RayCollisionList CollideClosest(const CRAB::Ray &ray);
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray);
};

