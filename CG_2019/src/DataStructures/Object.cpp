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

float Object::Collide(register const CRAB::Ray &ray) const {
	return geometry->CollideClosest(ray);
}

CRAB::RayCollisionList Object::CollideAll(const CRAB::Ray &ray) const {
	return geometry->Collide(ray);
}

CRAB::Vector4Df Object::getColor() const {
	return color;
}

Geometry* Object::getGeometry() const {
	return geometry;
}