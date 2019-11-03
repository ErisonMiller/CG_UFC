#include "Material.h"

Material::Material()
{

}

Material::Material(CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess)
{
	this->ka = ka;
	this->kd = kd;
	this->ks = ks;
	this->shininess = shininess;
	alfa = 1.0f;
	reflection = 0.0f;
}
Material::Material(CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess, float _alpha, float _reflection)
{
	this->ka = ka;
	this->kd = kd;
	this->ks = ks;
	this->shininess = shininess;
	alfa = _alpha;
	reflection = _reflection;
}
Material::~Material()
{

}