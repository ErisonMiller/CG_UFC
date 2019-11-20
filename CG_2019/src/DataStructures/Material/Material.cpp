#include "Material.h"

Material::Material()
{

}

Material::Material(const std::string &name, CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess)
{
	this->name = name;
	this->ka = ka;
	this->kd = kd;
	this->ks = ks;
	this->shininess = shininess;
	alfa = 1.0f;
	reflection = 0.0f;
}

Material::Material(const std::string &name, CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess, float _alpha, float _reflection)
{
	this->name = name;
	this->ka = ka;
	this->kd = kd;
	this->ks = ks;
	this->shininess = shininess;
	alfa = _alpha;
	reflection = _reflection;
}

Material::Material(const std::string &name, CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess, float _ior)
{
	this->name = name;
	this->ka = ka;
	this->kd = kd;
	this->ks = ks;
	this->shininess = shininess;
	ior = _ior;
}

Material::Material(const std::string &name, CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float shininess, float _alpha, float _reflection, float _ior)
{
	this->name = name;
	this->ka = ka;
	this->kd = kd;
	this->ks = ks;
	this->shininess = shininess;
	alfa = _alpha;
	reflection = _reflection;
	ior = _ior;
}

Material::~Material()
{

}

std::string Material::getName() const {
	return name;
}