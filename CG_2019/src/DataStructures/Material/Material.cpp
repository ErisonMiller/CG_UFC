#include "Material.h"

Material::Material()
{

}

Material::Material(CRAB::Vector4Df ka, CRAB::Vector4Df kd, CRAB::Vector4Df ks)
{
	this->ka = ka;
	this->kd = kd;
	this->ks = ks;
}

Material::~Material()
{

}