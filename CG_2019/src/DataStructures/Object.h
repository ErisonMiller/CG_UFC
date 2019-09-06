#pragma once
#include <string>
#include "Geometry/Geometry.h"
#include "Material/Material.h"
#include "Linear_Algebra.h"

using namespace std;

class Object
{
	Geometry *geometry;
	Material *material;
public:
	string name;

	bool visible = false;

	Object();
	Object(Geometry *_geometry);
	Object(const string &_name, Material * _material, Geometry *_geometry);

	~Object();

	float Collide(register const CRAB::Ray &ray) const;
	CRAB::RayCollisionList CollideAll(const CRAB::Ray &ray) const;

	string getName() const;
	Geometry* getGeometry() const;
	Material* getMaterial() const;
};

