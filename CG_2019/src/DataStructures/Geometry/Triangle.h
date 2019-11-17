#pragma once
#include "Geometry.h"

struct Face
{
	CRAB::Vector4Df v1, v2, v3;
	CRAB::Vector4Df getCenter()const { return (v1 + v2 + v3)/3; }
};
#define FaceList std::vector<Face>

class Triangle :
	public Geometry
{
public:

	//union {
	//	struct { CRAB::Vector4Df v1, n_e1, n_e2, normal; };
	//	__m256 v256[2];
	//};
	CRAB::Vector4Df v1, n_e1, n_e2, normal, v_n1, v_n2, v_n3;

	Triangle(const CRAB::Vector4Df &v1, const CRAB::Vector4Df &v2, const CRAB::Vector4Df &v3);
	Triangle(const CRAB::Vector4Df& v1, const CRAB::Vector4Df& v2, const CRAB::Vector4Df& v3,
		const CRAB::Vector4Df _vn1, const CRAB::Vector4Df _vn2, const CRAB::Vector4Df _vn3);
	Triangle(const Triangle &tri);
	Triangle();
	~Triangle();

	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray> &ray);
	CRAB::Collision CollideClosest(const CRAB::Ray &ray);
	CRAB::RayCollisionList Collide(const CRAB::Ray &ray);

	CRAB::Vector4Df getNormal(const CRAB::Vector4Df &point);

	void transform(CRAB::Matrix4 m);
};

