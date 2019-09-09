#include "PointLight.h"

PointLight::PointLight(CRAB::Vector4Df intensity, CRAB::Vector4Df _position) : Light(intensity), position(_position) {}

PointLight::~PointLight() {}

CRAB::Vector4Df PointLight::Illumination(const Material& mat, const CRAB::Vector4Df& normal, const CRAB::Vector4Df& view, const CRAB::Vector4Df& P)
{
	// light vector
	CRAB::Vector4Df L = position - P;
	L.normalize();

	// color
	CRAB::Vector4Df color;
	if (dot(L, normal) > 0.0f)
		color = this->intensity;
	else
		color = {0.0f, 0.0f, 0.0f, 0};

	// reflection vector
	CRAB::Vector4Df r = reflection(L, normal);

	CRAB::Vector4Df Ia = color * mat.ka;                                   // ambient reflection
	CRAB::Vector4Df Id = (color * mat.kd) * dot(L, normal);                // diffuse reflection
	CRAB::Vector4Df Is = (color * mat.ks) * powf(dot(r, view), mat.alfa);  // specular reflection

	return Ia + Id + Is;
}