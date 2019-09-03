#include "Object.h"


Object::Object()
{
}
Object::Object(Geometry *_geometry): geometry(_geometry)
{
	name = "untitled"; // default name
	color = { 0.0f, 0.0f, 0.0f, 0 }; // default color
}

Object::Object(const string &_name, const CRAB::Vector4Df &_color, Geometry *_geometry) : name(_name), geometry(_geometry), color(_color), visible(false)
{
}

Object::~Object()
{
}

float Object::Collide(register const CRAB::Ray &ray) const {
	//CRAB::RayCollisionList Coll = geometry->Collide(ray);
	//if (Coll.collisions.size() == 0)return INFINITY;
	//return Coll.collisions[0].distance;
	return geometry->CollideClosest(ray);
}

CRAB::RayCollisionList Object::CollideAll(const CRAB::Ray &ray) const {
	return geometry->Collide(ray);
}

string Object::getName() const {
	return name;
}

Geometry* Object::getGeometry() const {
	return geometry;
}