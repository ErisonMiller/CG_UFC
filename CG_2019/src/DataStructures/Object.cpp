#include "Object.h"


Object::Object()
{
}
Object::Object(Geometry *_geometry): geometry(_geometry)
{
}

Object::Object(const CRAB::Vector4Df &_color, Geometry *_geometry) : geometry(_geometry), color(_color)
{
}

Object::~Object()
{
}

float Object::Collide(const CRAB::Ray &ray) {
	if (geometry->Collide(ray).collisions.size() == 0)return INFINITY;
	return geometry->Collide(ray).collisions[0].distance;
}

CRAB::RayCollisionList Object::CollideAll(const CRAB::Ray &ray) {
	return geometry->Collide(ray);
}

CRAB::Vector4Df Object::getColor() {
	return color;
}
