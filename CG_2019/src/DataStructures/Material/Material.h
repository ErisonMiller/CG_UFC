#pragma once
#include "Linear_Algebra.h"

class Material
{
public:
	CRAB::Vector4Df ka; // Ambient Coefficient
	CRAB::Vector4Df kd; // Diffuse Coefficient
	CRAB::Vector4Df ks; // Spectral Coefficient

	Material();
	Material(CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks);
	~Material();
};

