#pragma once

#include "Linear_Algebra.h"
#include "Triangle.h"
#include <vector>

#define TREE_EMPTY_NODE		0
#define TREE_FULL_NODE		1
#define TREE_SEMI_NODE		2


class Octree
{
public:
	Octree();
	~Octree();
};



struct TreeElement
{
	int state;

	TreeElement(int _state) : state(_state) {}
};


struct Tree
{
	CRAB::Vector4Df position;
	float size;
	TreeElement *tree;
};


struct QuadElementNode : TreeElement
{
	TreeElement *sons[4];
	QuadElementNode() : TreeElement(TREE_SEMI_NODE) {}
};


struct OcElementNode : TreeElement
{
	TreeElement *sons[8];
	OcElementNode() : TreeElement(TREE_SEMI_NODE) {}
};

struct Face {
	CRAB::Vector4Df v1, v2, v3;
};

class Ocject {
public:
	CRAB::Vector4Df min, max, center;
	float r; int sons;
	std::vector<Triangle> triangles;
	std::vector<Ocject *> filhos;

	Ocject();
	Ocject(std::vector<Face> &fs);
	Ocject(std::vector<Face> &fs, const CRAB::Vector4Df &cmin, const CRAB::Vector4Df &cmax, const CRAB::Vector4Df &ccenter);

	inline void Subdivide(std::vector<Face> &faces);
};
