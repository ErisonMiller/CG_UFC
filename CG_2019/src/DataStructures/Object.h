#pragma once

#include "Geometry/Geometry.h"

class Object
{
	Geometry *geometry;
public:
	Object();
	Object(Geometry *_geometry);

	~Object();
	
	float Collide(const CRAB::Ray &ray);
};

