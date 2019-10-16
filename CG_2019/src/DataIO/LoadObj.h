#pragma once

// Iostream - STD I/O Library
#include <iostream>

// String - STD String Library
#include <string>

// fStream - STD File I/O Library
#include <fstream>


#include "DataStructures/Linear_Algebra.h"
#include "Geometry/Triangle.h"

// Namespace: OBJL
//
// Description: The namespace that holds eveyrthing that
//	is needed and used for the OBJ Model Loader
namespace CRAB
{
	#define FaceList std::vector<Triangle>

	std::vector<FaceList> Load_Obj(std::string &name) {
		std::vector<FaceList> objs;
		std::vector<Vector4Df> vertices;

		std::ifstream infile(name);

		std::string tag;
		int num_objs = -1;
		int f1, f2, f3;
		float x, y, z;
		float w = 1.0f;

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
			if (tag == "f") {
				infile >> f1;
				infile >> f2;
				infile >> f3;

				objs[num_objs].push_back(Triangle(vertices[f1 - 1], vertices[f2 - 1], vertices[f3 - 1] ));
				continue;
			}

			getline(infile, tag);
			continue;
		}

		infile.close();

		return objs;
	}
}