#pragma once

#include "Geometry/Geometry.h"


class Object
{
	Geometry *geometry;
	CRAB::Vector4Df color;
public:

	bool visible = false;

	Object();
	Object(Geometry *_geometry);
	Object(const CRAB::Vector4Df &_color, Geometry *_geometry);

	~Object();
	
	float Collide(register const CRAB::Ray &ray) const;
	CRAB::RayCollisionList CollideAll(const CRAB::Ray &ray) const;

	CRAB::Vector4Df getColor() const;
	Geometry* getGeometry() const;
};

