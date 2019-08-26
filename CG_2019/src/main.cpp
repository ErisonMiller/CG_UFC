//System Include
#include <iostream>

//User Include Files
#include "Cylinder.h"
#include "Cone.h"

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
	ray_origin.x = 10;
	ray_origin.y = 5;
	ray_origin.z = 0;

	//Ray Direction
	ray_dir.x = -1;
	ray_dir.y = 0;
	ray_dir.z = 0;

	ray_dir = ray_dir.to_unitary();

	CRAB::Ray ray;

	ray.origin = ray_origin;
	ray.direction = ray_dir;

	//Collisions List
	CRAB::RayCollisionList colList;

#if CYLINDER_TEST == 1

	//Cylinder
	CRAB::Vector4Df cylinder_base_center, cylinder_dir;
	float H_cylinder = 100.0;
	float R_cylinder = 5.0;

	//Base Center
	cylinder_base_center.x = 0;
	cylinder_base_center.y = 0;
	cylinder_base_center.z = 0;

	//Direction
	cylinder_dir.x = 0;
	cylinder_dir.y = 1;
	cylinder_dir.z = 0;

	cylinder_dir = cylinder_dir.to_unitary(); // Unitary value

	Cylinder cylinder = Cylinder(H_cylinder, R_cylinder, cylinder_base_center, cylinder_dir);

	//Intersection

	colList = cylinder.CollideAll(ray);

	std::cout << "intersections Number:" << colList.collisions.size() << "\n";

#endif

#if CONE_TEST == 1

	//Cylinder
	CRAB::Vector4Df cone_base_center, cone_dir;
	float H_cone = 100.0;
	float R_cone = 5.0;

	//Base Center
	cone_base_center.x = 0;
	cone_base_center.y = 0;
	cone_base_center.z = 0;

	//Direction
	cone_dir.x = 0;
	cone_dir.y = 1;
	cone_dir.z = 0;

	cone_dir = cone_dir.to_unitary(); // Unitary value

	Cone cone = Cone(H_cone, R_cone, cone_base_center, cone_dir);

	//Intersection

	colList = cone.CollideAll(ray);

	std::cout << "intersections Number:" << colList.collisions.size() << "\n";

#endif

	std::cin >> buff;

	return 0;
}