#pragma once
#include "Geometry.h"
#include "Quad.h"

class Cube :
	public Geometry
{
	Quad quads[6];
	CRAB::Vector4Df center;
	float r2;
public:
	Cube();
	Cube(const CRAB::Vector4Df &base, const CRAB::Vector4Df &up, const CRAB::Vector4Df &front, float size);

	~Cube();

	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray);
	CRAB::Collision CollideClosest(register const CRAB::Ray &ray);
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray);

	CRAB::Vector4Df getNormal(const CRAB::Vector4Df& point);

	void tranform(CRAB::Matrix4 m);
};