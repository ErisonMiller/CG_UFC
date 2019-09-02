#pragma once

#include "Geometry/Geometry.h"


class Object
{
	Geometry *geometry;
public:
	CRAB::Vector4Df color;

	Object();
	Object(Geometry *_geometry);
	Object(const CRAB::Vector4Df &_color, Geometry *_geometry);

	~Object();
	
	float Collide(register const CRAB::Ray &ray) const;
	CRAB::RayCollisionList CollideAll(const CRAB::Ray &ray) const;

	Geometry* getGeometry() const;
};

