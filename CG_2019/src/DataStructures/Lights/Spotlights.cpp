#include "Spotlights.h"


Spotlights::Spotlights()
{
}

Spotlights::Spotlights(CRAB::Vector4Df intensity, CRAB::Vector4Df _position) : Light(intensity), position(_position)
{
}

Spotlights::~Spotlights()
{
}

CRAB::Vector4Df Spotlights::Illumination(const Material &mat, const CRAB::Vector4Df &normal, const CRAB::Vector4Df &view, const CRAB::Vector4Df &P) {
	CRAB::Vector4Df color;

	float d2 = (P - position).lengthsq(); //square of the distance from the source
	//color = this->intensity / d2; //intensity from a point source located at 'position'

	//color = color * mat.ka; //ambient reflection
	//diffuse reflection
	//specular reflection

	color = this->intensity * mat.ka;
	return color;
}