#include "Material.h"

Material::Material()
{

}

Material::Material(CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks, float alfa)
{
	this->ka = ka;
	this->kd = kd;
	this->ks = ks;
	this->alfa = alfa;
}

Material::~Material()
{

}