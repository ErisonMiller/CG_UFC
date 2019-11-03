#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
public:
	CRAB::Vector4Df direction;
	DirectionalLight();

	DirectionalLight(CRAB::Vector4Df _intensity, CRAB::Vector4Df _direction);

	~DirectionalLight();

	CRAB::Vector4Df Illumination(const Material &mat, const CRAB::Vector4Df &normal, const CRAB::Vector4Df &view, const CRAB::Vector4Df &P) const;
	float LightDistance(const CRAB::Vector4Df& point) const;
	CRAB::Vector4Df GetLightDirection(const CRAB::Vector4Df& point) const;
};

