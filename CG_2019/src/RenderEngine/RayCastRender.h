#pragma once
#include "Camera.h"
#include "Object.h"


class RayCast{
	const CRAB::float2 resolution; 
	CRAB::Vector4Df *accumulateBuffer;
	
	public:

	RayCast(const CRAB::Camera &cam);

	CRAB::Vector4Df* Render(const CRAB::Camera &cam, std::vector<Object> &objects);
};