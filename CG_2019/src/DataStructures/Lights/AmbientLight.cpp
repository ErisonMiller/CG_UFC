#include "AmbientLight.h"

AmbientLight::AmbientLight()
{

}

AmbientLight::AmbientLight(CRAB::Vector4Df intensity) : Light(intensity)
{
}

AmbientLight::~AmbientLight()
{

}

CRAB::Vector4Df AmbientLight::Illumination(const Material &mat, const CRAB::Vector4Df &normal, const CRAB::Vector4Df &view, const CRAB::Vector4Df &P) const {
	CRAB::Vector4Df color;

	color = this->intensity * mat.ka;

	return color;
}

float AmbientLight::LightDistance(const CRAB::Vector4Df& point) const {
	return 0;
}
