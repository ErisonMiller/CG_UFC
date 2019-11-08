#include "Light.h"



Light::Light()
{
}

Light::Light(CRAB::Vector4Df intensity)
{
	this->intensity = intensity;
}


Light::~Light()
{
}

void Light::TurnOn(){
	this->on = 1;
}

void Light::TurnOff(){
	this->on = 0;
}