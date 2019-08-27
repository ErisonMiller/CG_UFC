//System Include
#include <iostream>

//User Include Files
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"

#include "WindowController/WindowController.h"

//Test Defines
#define CYLINDER_TEST 0 // Set to 1 to run the Cylinder Test
#define CONE_TEST 0 // Set to 1 to run the Cone Test

int main(int argc, char** argv) {
	//this later will call the window controller
	//now is just for tests

	char buff;

	//Ray
	CRAB::Vector4Df ray_origin, ray_dir;

	//Ray Origin
	ray_origin.x = 0;
	ray_origin.y = 0;
	ray_origin.z = 4;

	//Ray Direction
	ray_dir.x = 0;
	ray_dir.y = 0.57;
	ray_dir.z = -2;

	ray_dir = ray_dir.to_unitary();

	CRAB::Ray ray;

	std::vector<CRAB::Ray> ray_vec;

	ray.origin = ray_origin;
	ray.direction = ray_dir;

	//Collisions List
	CRAB::RayCollisionList colList;

#if CYLINDER_TEST == 1

	//Cylinder
	CRAB::Vector4Df cylinder_base_center, cylinder_dir;
	float H_cylinder = 4.0;
	float R_cylinder = 0.5;

	//Base Center
	cylinder_base_center.x = 0;
	cylinder_base_center.y = 0;
	cylinder_base_center.z = -10;

	//Direction
	cylinder_dir.x = 0;
	cylinder_dir.y = 1;
	cylinder_dir.z = 0;

	cylinder_dir = cylinder_dir.to_unitary(); // Unitary value

	Cylinder cylinder = Cylinder(H_cylinder, R_cylinder, cylinder_base_center, cylinder_dir);

	//Intersection

	//Collide one Ray
	colList = cylinder.Collide(ray);

	std::cout << "Cylinder one ray Intersections Number:" << colList.collisions.size() << "\n";
	for (int i = 0; i < colList.collisions.size(); i++)
		std::cout << "t" << i + 1 << " = (" << colList.collisions[i].pint.x << ";" << colList.collisions[i].pint.y << ";" << colList.collisions[i].pint.z << ")" << std::endl;

	//Collide more than one ray

	ray_vec.clear();
	for (int i = 0; i < 10; i++)
	{
		ray.origin.y = ray_origin.y * i;
		ray.direction = ray_dir;
		ray_vec.push_back(ray);
	}

	colList = cylinder.CollideAll(ray_vec);

	std::cout << "Cylinder more than one ray Intersections Number:" << colList.collisions.size() << "\n";

#endif

#if CONE_TEST == 1

	//Cylinder
	CRAB::Vector4Df cone_base_center, cone_dir;
	float H_cone = 9.0;
	float R_cone = 2.0;

	//Base Center
	cone_base_center.x = 0;
	cone_base_center.y = 4;
	cone_base_center.z = -10;

	//Direction
	cone_dir.x = 0;
	cone_dir.y = 1;
	cone_dir.z = 0;

	cone_dir = cone_dir.to_unitary(); // Unitary value

	Cone cone = Cone(H_cone, R_cone, cone_base_center, cone_dir);

	//Intersection

	colList = cone.Collide(ray);

	std::cout << "Cone one ray Intersections Number:" << colList.collisions.size() << "\n";
	for (int i = 0; i < colList.collisions.size(); i++)
		std::cout << "t" << i + 1 << " = (" << colList.collisions[i].pint.x << ";" << colList.collisions[i].pint.y << ";" << colList.collisions[i].pint.z << ")" << std::endl;

	//Collide more than one ray
	ray_vec.clear();
	for (int i = 0; i < 10; i++)
	{
		ray.origin.y = ray_origin.y * i;
		ray.direction = ray_dir;
		ray_vec.push_back(ray);
	}

	colList = cone.CollideAll(ray_vec);

	std::cout << "Cone more than one ray Intersections Number:" << colList.collisions.size() << "\n";

#endif

	//start the window and the rendering
	Start_Window(argc, argv);
	std::cin >> buff;

	return 0;
}