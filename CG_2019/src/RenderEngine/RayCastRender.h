#pragma once
#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include "Sphere.h"
//Class to render using raycast

class RayCast{
	CRAB::float2 resolution; 
	CRAB::Vector4Df *accumulateBuffer;
	
	public:

	RayCast();
	RayCast(const CRAB::Camera &cam);

	~RayCast();

	//return a Vector4df matrix where each Vector4Df is a color
	CRAB::Vector4Df* Render(const CRAB::Camera &cam, const std::vector<Object> &objects, std::vector<Light *> &lights);
	CRAB::Vector4Df* Render(const CRAB::Camera &cam, const std::vector<Sphere> &spheres);

	Object* RayPick(const CRAB::Camera &cam, std::vector<Object> &objects, int x, int y);

};