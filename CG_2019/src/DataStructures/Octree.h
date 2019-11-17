#pragma once

#include "Linear_Algebra.h"
#include "Triangle.h"
#include <vector>

#define TREE_EMPTY_NODE		2
#define TREE_FULL_NODE		0
#define TREE_SEMI_NODE		1



struct TreeElement
{
	int state;
	TreeElement* next;

	//CRAB::Vector4Df center;
	//float r2;
	CRAB::Vector4Df min, max;

	TreeElement(int _state) : state(_state), next(nullptr) {}

	void Free() { if (next) { next->Free(); } delete next; }
};


struct OcElementNode : TreeElement
{
	TreeElement* sons[8];
	OcElementNode() : TreeElement(TREE_SEMI_NODE) {}

	void Free() { for (TreeElement* t : sons) { t->Free(); delete t; } }
};

struct OcElementNodeCache : TreeElement
{
	TreeElement* son;
	OcElementNodeCache() : TreeElement(TREE_SEMI_NODE) {}

	void Free() { son->Free(); delete son; next->Free(); delete next; }
};


struct OcElementLeaf : TreeElement
{
	std::vector<Triangle> faces;

	OcElementLeaf() : TreeElement(TREE_FULL_NODE) {}
	OcElementLeaf(const FaceList& _faces) : TreeElement(TREE_FULL_NODE) {
		faces.reserve(_faces.size() * 0.5f);
		int j = 0;
		for (int i = 0, s = _faces.size(); i < s; i += 2) {
			const Face& f = _faces[i];
			const Face& n = _faces[i + 1];
			//faces[j] = (Triangle(f.v1, f.v2, f.v3, &vertice_normals[j]));
			faces.push_back(Triangle(f.v1, f.v2, f.v3, n.v1, n.v2, n.v3));
			//faces[j] = (Triangle(f.v1, f.v2, f.v3));
			//faces[j].v_n1 = _faces[i + 1].v1;
			//faces[j].v_n2 = _faces[i + 1].v2;
			//faces[j].v_n3 = _faces[i + 1].v3;
			//
		}
	}

};


class OcTree : public Geometry {
public:
	CRAB::Vector4Df center, min, max;
	float r;
	TreeElement* tree;
	std::vector<Face> vertices_normals;


	OcTree(const FaceList& faceList);

	CRAB::Collision CollideClosest(const CRAB::Ray& ray);//return the closest collision distance of a ray and the geometry
	CRAB::RayCollisionList CollideAll(const std::vector<CRAB::Ray>& ray) { return CRAB::RayCollisionList(); };//return all the colisions of a set of rays
	CRAB::RayCollisionList Collide(const CRAB::Ray& ray) { return CRAB::RayCollisionList(); };//return all collisions of a ray

	CRAB::Vector4Df getNormal(const CRAB::Vector4Df& point) { return CRAB::Vector4Df{ 1,0,0,0 }; } // returns the normal vector at point on a surface

	void transform(CRAB::Matrix4 m) {}; // Applies the tranform matrix



	void Free() { tree->Free(); delete tree; }

};
