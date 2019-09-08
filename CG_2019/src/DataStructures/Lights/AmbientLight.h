#pragma once

#include "Light.h"

class AmbientLight : public Light
{

public:
	AmbientLight();
	AmbientLight(CRAB::Vector4Df intensity);
	~AmbientLight();

	CRAB::Vector4Df Illumination(const Material &mat, const CRAB::Vector4Df &normal, const CRAB::Vector4Df &view, const CRAB::Vector4Df &P);
};

