#include <iostream>
#include "Cylinder.h"
#define CYLINDER_TEST 0 // Set to 1 to run the Cylinder Test

int main(int argc, char** argv) {
	//this later will call the window controller
	//now is just for tests

#if CYLINDER_TEST == 1
	char buff;

	//Cylinder
	CRAB::Vector4Df cylinder_base_center, cylinder_dir;
	float H = 100.0;
	float R = 5.0;

	//Base Center
	cylinder_base_center.x = 0;
	cylinder_base_center.y = 0;
	cylinder_base_center.z = 0;

	//Direction
	cylinder_dir.x = 0;
	cylinder_dir.y = 1;
	cylinder_dir.z = 0;

	cylinder_dir = cylinder_dir.to_unitary(); // Unitary value

	Cylinder cylinder = Cylinder(H, R, cylinder_base_center, cylinder_dir);

	//Ray
	CRAB::Vector4Df ray_origin, ray_dir;

	//Ray Origin
	ray_origin.x = 0;
	ray_origin.y = 10;
	ray_origin.z = 10;

	//Ray Direction
	ray_dir.x = 0;
	ray_dir.y = 0;
	ray_dir.z = 1;

	ray_dir = ray_dir.to_unitary();

	CRAB::Ray ray;

	ray.origin    = ray_origin;
	ray.direction = ray_dir;


	//Intersection
	CRAB::RayCollisionList colList;

	colList = cylinder.CollideAll(ray);

	std::cout << "Inserction Number:" << colList.collisions.size() << "\n";

	std::cin >> buff;

#endif
	return 0;
}