#pragma once
#include <string>
#include "Linear_Algebra.h"

class Material
{
public:
	std::string name;
	CRAB::Vector4Df ka; // Ambient Coefficient
	CRAB::Vector4Df kd; // Diffuse Coefficient
	CRAB::Vector4Df ks; // Spectral Coefficient
	float shininess;	// Shininess
	float alfa;			// Transparency [0.0; 1.0]
	float reflection;	// [0.0; 1.0]
	float ior = 1;		//Refractive Index

	Material();
	Material(const std::string &name, CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess);
	Material(const std::string &name, CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess, float _alpha, float _reflection);
	Material(const std::string &name, CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess, float _ior);
	Material(const std::string &name, CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess, float _alpha, float _reflection, float _ior);

	~Material();

	std::string getName() const;
};

