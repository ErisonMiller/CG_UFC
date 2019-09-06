#pragma once
#include "Linear_Algebra.h"

class Material
{
public:
	CRAB::Vector4Df mat_properties;

	Material();
	Material(CRAB::Vector4Df mat_properties);
	~Material();
};

