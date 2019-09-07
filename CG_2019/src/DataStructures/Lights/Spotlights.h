#pragma once

#include "Light.h"

class Spotlights : public Light
{
	CRAB::Vector4Df position; // point source
public:
	Spotlights();
	Spotlights(CRAB::Vector4Df intensity, CRAB::Vector4Df _position);
	~Spotlights();

	CRAB::Vector4Df Illumination(const Material &mat, const CRAB::Vector4Df &normal, const CRAB::Vector4Df &view, const CRAB::Vector4Df &P);
};

