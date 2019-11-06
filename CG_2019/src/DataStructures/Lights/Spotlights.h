#pragma once

#include "Light.h"

class Spotlights : public Light
{
public:
	CRAB::Vector4Df position;		// point source
	CRAB::Vector4Df direction;	// spot direction
	float angle;			// spot angle
	float exponent;			// spot exponent

	Spotlights();
	Spotlights(CRAB::Vector4Df intensity, CRAB::Vector4Df _position, CRAB::Vector4Df _direction, float _angle, float _exp);
	~Spotlights();

	CRAB::Vector4Df Illumination(const Material &mat, const CRAB::Vector4Df &normal, const CRAB::Vector4Df &view, const CRAB::Vector4Df &P) const;
	float LightDistance(const CRAB::Vector4Df& point) const;
};

