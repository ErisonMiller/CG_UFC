#include "DirectionalLight.h"



DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(CRAB::Vector4Df _intensity, CRAB::Vector4Df _direction): 
	direction(_direction.to_unitary()){
	intensity = _intensity;

}

DirectionalLight::~DirectionalLight()
{
}

CRAB::Vector4Df DirectionalLight::Illumination(const Material &mat, const CRAB::Vector4Df &normal, const CRAB::Vector4Df &view, const CRAB::Vector4Df &P) {

	// the direction of a perfect reflector
	CRAB::Vector4Df r = reflection(direction, normal);

	CRAB::Vector4Df Ia = intensity * mat.ka; //ambient reflection
	CRAB::Vector4Df Id = CRAB::Vector4Df{ 0,0,0,0 };
	float dot_d_n = dot(direction, normal);
	if (dot_d_n > 0) {
		Id = (intensity * mat.kd) * dot_d_n; //diffuse reflection

		
	}
	float dot_r_v = dot(r, view);
	if (dot_r_v > 0) {
		Id = Id + (intensity * mat.ks) * powf(dot_r_v, mat.alfa);//specular reflection
	}
	return Ia + Id;
}
