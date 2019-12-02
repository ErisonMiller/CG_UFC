#pragma once

// Iostream - STD I/O Library
#include <iostream>

// String - STD String Library
#include <string>

// fStream - STD File I/O Library
#include <fstream>

#include "Light.h"
#include "PointLight.h"
#include "Spotlights.h"
#include "DirectionalLight.h"

#include "Material.h"

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

	std::vector<FaceList> Load_Obj(const std::string &name, std::vector<Light *> &lightList, const std::vector<Material *> &materialList, std::vector<Object> &objList) {
		std::vector<FaceList> objs;
		std::vector<Vector4Df> vertices;
		std::vector<Vector4Df> vertices_normal;

		std::ifstream infile(name);

		std::string tag, objName, mtl;
		int num_objs = -1;
		int f1, f2, f3, n1, n2, n3;
		float r, g, b, x, y, z, i, j, k, ang, exp, radius, height;
		float w = 1.0f;
		char c;

		while (infile >> tag) {
			if (tag == "o") {
				num_objs++;
				objs.push_back(FaceList());
				
				infile >> objName;
				objs[num_objs].name = objName;
				objs[num_objs].material = new Material(objName + "_Neutral", CRAB::Vector4Df{ 0.3f, 0.3f, 0.3f, 0.0f }, CRAB::Vector4Df{ 1.0f, 1.0f, 1.0f, 0.0f }, CRAB::Vector4Df{ 1.0f, 1.0f, 1.0f, 0.0f }, 10.0f);

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

				//objs[num_objs].push_back(Face{ vertices[f1 - 1], vertices[f2 - 1], vertices[f3 - 1] });
				objs[num_objs].faces.push_back(Face{ vertices[f1 - 1], vertices[f2 - 1], vertices[f3 - 1] });
				//objs[num_objs].push_back(Face{ vertices_normal[n1 - 1], vertices_normal[n2 - 1], vertices_normal[n3 - 1] });
				objs[num_objs].faces.push_back(Face{ vertices_normal[n1 - 1], vertices_normal[n2 - 1], vertices_normal[n3 - 1] });
				continue;
			}
			if (tag == "d") {
				infile >> r;
				infile >> g;
				infile >> b;
				infile >> x;
				infile >> y;
				infile >> z;
				lightList.push_back(new DirectionalLight(Vector4Df{ r, g, b, 0.0f }, Vector4Df{ x, y, z, 0.0f }));
				continue;
			}
			if (tag == "p") {
				infile >> r;
				infile >> g;
				infile >> b;
				infile >> x;
				infile >> y;
				infile >> z;
				lightList.push_back(new PointLight(Vector4Df{ r, g, b, 0.0f }, Vector4Df{ x, y, z, 1.0f }));
				continue;
			}
			if (tag == "sp") {
				infile >> r;
				infile >> g;
				infile >> b;
				infile >> x;
				infile >> y;
				infile >> z;
				infile >> i;
				infile >> j;
				infile >> k;
				infile >> ang;
				infile >> exp;
				lightList.push_back(new Spotlights(Vector4Df{ r, g, b, 0.0f }, Vector4Df{ x, y, z, 1.0f }, Vector4Df{ i, j, k, 0.0f }, ang, exp));
				continue;
			}
			if (tag == "m") {
				infile >> mtl;
				for (int n = 0; n < materialList.size(); n++)
					if (materialList[n]->name == mtl)
						//objs[num_objs].material = materialList[n];
						objs[num_objs].material = new Material(
							objs[num_objs].name + "_" + materialList[n]->name,
							materialList[n]->ka, 
							materialList[n]->kd, 
							materialList[n]->ks, 
							materialList[n]->shininess, 
							materialList[n]->alfa, 
							materialList[n]->reflection, 
							materialList[n]->ior);

				continue;
			}
			if (tag == "sphere") {
				infile >> objName;
				infile >> mtl;
				infile >> x;
				infile >> y;
				infile >> z;
				infile >> radius;
				for (int n = 0; n < materialList.size(); n++)
					if (materialList[n]->name == mtl) {
						objList.push_back(Object(objName, new Material(objName + "_" + materialList[n]->name, materialList[n]->ka, materialList[n]->kd, materialList[n]->ks, materialList[n]->shininess, materialList[n]->alfa, materialList[n]->reflection, materialList[n]->ior), new Sphere(Vector4Df{ x, y, z, 1.0f }, radius)));
						break;
					}
					else if (n == materialList.size() - 1)
						objList.push_back(Object(objName, new Material(objName + "_Neutral", CRAB::Vector4Df{ 0.3f, 0.3f, 0.3f, 0.0f }, CRAB::Vector4Df{ 1.0f, 1.0f, 1.0f, 0.0f }, CRAB::Vector4Df{ 1.0f, 1.0f, 1.0f, 0.0f }, 10.0f), new Sphere(Vector4Df{ x, y, z, 1.0f }, radius)));
				continue;
			}
			if (tag == "cylinder") {
				infile >> objName;
				infile >> mtl;
				infile >> height;
				infile >> radius;
				infile >> x;
				infile >> y;
				infile >> z;
				infile >> i;
				infile >> j;
				infile >> k;
				for (int n = 0; n < materialList.size(); n++)
					if (materialList[n]->name == mtl) {
						objList.push_back(Object(objName, new Material(objName + "_" + materialList[n]->name, materialList[n]->ka, materialList[n]->kd, materialList[n]->ks, materialList[n]->shininess, materialList[n]->alfa, materialList[n]->reflection, materialList[n]->ior), new Cylinder(height, radius, Vector4Df{ x, y, z, 1.0f }, Vector4Df{ i, j, k, 0.0f })));
						break;
					}
					else if (n == materialList.size() - 1)
						objList.push_back(Object(objName, new Material(objName + "_Neutral", CRAB::Vector4Df{ 0.3f, 0.3f, 0.3f, 0.0f }, CRAB::Vector4Df{ 1.0f, 1.0f, 1.0f, 0.0f }, CRAB::Vector4Df{ 1.0f, 1.0f, 1.0f, 0.0f }, 10.0f), new Cylinder(height, radius, Vector4Df{ x, y, z, 1.0f }, Vector4Df{ i, j, k, 0.0f })));
				continue;
			}
			if (tag == "cone") {
				infile >> objName;
				infile >> mtl;
				infile >> height;
				infile >> radius;
				infile >> x;
				infile >> y;
				infile >> z;
				infile >> i;
				infile >> j;
				infile >> k;
				for (int n = 0; n < materialList.size(); n++)
					if (materialList[n]->name == mtl) {
						objList.push_back(Object(objName, new Material(objName + "_" + materialList[n]->name, materialList[n]->ka, materialList[n]->kd, materialList[n]->ks, materialList[n]->shininess, materialList[n]->alfa, materialList[n]->reflection, materialList[n]->ior), new Cone(height, radius, Vector4Df{ x, y, z, 1.0f }, Vector4Df{ i, j, k, 0.0f })));
						break;
					}
					else if (n == materialList.size() - 1)
						objList.push_back(Object(objName, new Material(objName + "_Neutral", CRAB::Vector4Df{ 0.3f, 0.3f, 0.3f, 0.0f }, CRAB::Vector4Df{ 1.0f, 1.0f, 1.0f, 0.0f }, CRAB::Vector4Df{ 1.0f, 1.0f, 1.0f, 0.0f }, 10.0f), new Cone(height, radius, Vector4Df{ x, y, z, 1.0f }, Vector4Df{ i, j, k, 0.0f })));
				continue;
			}

			getline(infile, tag);
			continue;
		}

		infile.close();

		return objs;
	}
}