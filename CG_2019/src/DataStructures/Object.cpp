#include "Object.h"


Object::Object()
{
}
Object::Object(Geometry *_geometry): geometry(_geometry)
{
}

Object::~Object()
{
}

float Object::Collide(const CRAB::Ray &ray) {
	if (geometry->Collide(ray).collisions.size() == 0)return INFINITY;
	return geometry->Collide(ray).collisions[0].distance;
}
