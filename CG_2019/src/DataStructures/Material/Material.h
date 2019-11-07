#pragma once
#include "Linear_Algebra.h"

class Material
{
public:
	CRAB::Vector4Df ka; // Ambient Coefficient
	CRAB::Vector4Df kd; // Diffuse Coefficient
	CRAB::Vector4Df ks; // Spectral Coefficient
	float shininess;   // Shininess
	float alfa;   // Shininess
	float reflection;
	float ior = 1; //Refractie Index

	Material();
	Material(CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess);
	Material(CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess, float _alpha, float _reflection);
	Material(CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess, float _ior);
	Material(CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess, float _alpha, float _reflection, float _ior);

	~Material();
};

