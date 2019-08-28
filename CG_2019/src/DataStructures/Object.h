#pragma once

#include "Geometry/Geometry.h"


class Object
{
	Geometry *geometry;
	CRAB::Vector4Df color;
public:

	Object();
	Object(Geometry *_geometry);
	Object(const CRAB::Vector4Df &_color, Geometry *_geometry);

	~Object();
	
	float Collide(const CRAB::Ray &ray);
	CRAB::RayCollisionList CollideAll(const CRAB::Ray &ray);

	CRAB::Vector4Df getColor();
	Geometry* getGeometry();
};

