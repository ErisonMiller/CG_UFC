#pragma once
#include "Linear_Algebra.h"
#include "Material.h"

class Light
{
public:
	CRAB::Vector4Df intensity;

	Light();
	Light(CRAB::Vector4Df intensity);
	virtual ~Light() = 0;

	//Return the collor after the interation with the light and material
	virtual CRAB::Vector4Df Illumination(const Material &mat, const CRAB::Vector4Df &normal, const CRAB::Vector4Df &view) = 0;
};