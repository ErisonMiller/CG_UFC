#pragma once

// Iostream - STD I/O Library
#include <iostream>

// String - STD String Library
#include <string>

// fStream - STD File I/O Library
#include <fstream>


#include "DataStructures/Linear_Algebra.h"
#include "Octree.h"
// Namespace: OBJL
//
// Description: The namespace that holds eveyrthing that
//	is needed and used for the OBJ Model Loader
namespace CRAB
{

	//struct Face
	//{
	//	CRAB::Vector4Df v1, v2, v3;
	//};
	//
	//#define FaceList std::vector<Face>

	//#define FaceList std::vector<Triangle>

	std::vector<FaceList> Load_Obj(const std::string &name) {
		std::vector<FaceList> objs;
		std::vector<Vector4Df> vertices;
		std::vector<Vector4Df> vertices_normal;

		std::ifstream infile(name);

		std::string tag;
		int num_objs = -1;
		int f1, f2, f3, n1, n2, n3;
		float x, y, z;
		float w = 1.0f;
		char c;

		while (infile >> tag) {
			if (tag == "o") {
				num_objs++;
				objs.push_back(FaceList());
				
				continue;
			}
			if (tag == "v") {
				//num_vertices++;
				infile >> x;
				infile >> y;
				infile >> z;
				vertices.push_back(Vector4Df{ x, y, z, w });
				continue;
			}
			if (tag == "vn") {
				//num_vertices++;
				infile >> x;
				infile >> y;
				infile >> z;
				vertices_normal.push_back(Vector4Df{ x, y, z, 0.0f });
				continue;
			}
			if (tag == "f") {
				infile >> f1 >> c >> c >> n1;
				infile >> f2 >> c >> c >> n2;
				infile >> f3 >> c >> c >> n3;
				//face vertices
				objs[num_objs].push_back(Face{ vertices[f1 - 1], vertices[f2 - 1], vertices[f3 - 1] });
				//normals
				objs[num_objs].push_back(Face{ vertices_normal[n1 - 1], vertices_normal[n2 - 1], vertices_normal[n3 - 1] });
				continue;
			}

			getline(infile, tag);
			continue;
		}

		infile.close();

		return objs;
	}
}