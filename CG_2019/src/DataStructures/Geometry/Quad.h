#pragma once
#include "Geometry.h"
class Quad :
	public Geometry
{
	
public:
	CRAB::Vector4Df v1, e1, e2;
	float size1, size2;

	Quad();

	Quad(const CRAB::Vector4Df &_v1, const CRAB::Vector4Df &_v2, const CRAB::Vector4Df &_v3, const CRAB::Vector4Df &_v4);
	~Quad();

	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray);
	CRAB::Collision CollideClosest(register const CRAB::Ray &ray);
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray);

	CRAB::Vector4Df getNormal(const CRAB::Vector4Df &point);

	void tranform(CRAB::Matrix4 m);
};

