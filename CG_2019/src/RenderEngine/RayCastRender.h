#pragma once
#include "Camera.h"
#include "Object.h"

//Class to render using raycast

class RayCast{
	const CRAB::float2 resolution; 
	CRAB::Vector4Df *accumulateBuffer;
	
	public:

	RayCast(const CRAB::Camera &cam);

	//return a Vector4df matrix where each Vector4Df is a color
	CRAB::Vector4Df* Render(const CRAB::Camera &cam, std::vector<Object> &objects);
};