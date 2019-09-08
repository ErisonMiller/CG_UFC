#pragma once
#include <string>
#include "Geometry/Geometry.h"

using namespace std;

class Object
{
	Geometry *geometry;
public:
	string name;
	CRAB::Vector4Df color;
	CRAB::Matrix4 transformation = CRAB::Identity();

	bool visible = false;

	Object();
	Object(Geometry *_geometry);
	Object(const string &_name, const CRAB::Vector4Df &_color, Geometry *_geometry);

	~Object();
	
	float Collide(register const CRAB::Ray &ray) const;
	CRAB::RayCollisionList CollideAll(const CRAB::Ray &ray) const;

	string getName() const;
	Geometry* getGeometry() const;
};

