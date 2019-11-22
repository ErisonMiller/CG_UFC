#include "Spotlights.h"


Spotlights::Spotlights()
{
}

Spotlights::Spotlights(CRAB::Vector4Df intensity, CRAB::Vector4Df _position, CRAB::Vector4Df _direction, float _angle, float _exp) : Light(intensity), position(_position), exponent(_exp)
{
	this->direction = _direction.to_unitary();
	this->angle = (_angle * M_PI) / 180.0f; // convert degree to radian
}

Spotlights::~Spotlights()
{
}

CRAB::Vector4Df Spotlights::Illumination(const Material &mat, const CRAB::Vector4Df &normal, const CRAB::Vector4Df &view, const CRAB::Vector4Df &P) const {
	CRAB::Vector4Df color;

	CRAB::Vector4Df L = position - P;
	float d2 = 0.5f * L.length(); //attenuation
	L.normalize();

	float spotCos = dot(direction, L*(-1.0f)); // angle between the direction of the source and a vector 'L' to a point on the surface
	float spotCosCutOff = cos(angle); // angular limit

	float dot_L_n = dot(L, normal);
	if (spotCos >= spotCosCutOff && dot_L_n > 0.0f) {
		color = this->intensity / d2; //intensity from a point source located at 'position'
		color = color * powf(spotCos, exponent);
	}
	else color = { 0.0f, 0.0f, 0.0f, 0 };


	//CRAB::Vector4Df Ia = color * mat.ka; //ambient reflection
	CRAB::Vector4Df Id = (color * mat.kd) * dot_L_n; //diffuse reflection
	CRAB::Vector4Df Is = { 0.0f, 0.0f, 0.0f, 0.0f };					  // specular reflection

	// the direction of a perfect reflector
	const CRAB::Vector4Df r = reflection(L, normal);
	float dot_r_v = dot(r, view);
	if (dot_r_v > 0.0f) {
		Is = (color * mat.ks) * powf(dot_r_v, mat.shininess);//specular reflection
	}

	return /*Ia +*/ Id + Is;
}


float Spotlights::LightDistance(const CRAB::Vector4Df& point) const {
	return (position - point).length();
}

CRAB::Vector4Df Spotlights::GetLightDirection(const CRAB::Vector4Df& point) const {
	return (position - point).to_unitary();
}