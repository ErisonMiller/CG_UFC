//System Include
#include <iostream>

//User Include Files
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "Cube.h"

#include "WindowController/WindowController.h"

//Test Defines
#define CYLINDER_TEST 0 // Set to 1 to run the Cylinder Test
#define CONE_TEST 0 // Set to 1 to run the Cone Test
#define SPHERE_TEST 0 // Set to 1 to run the Sphere Test
#define IMAGE_TEST 0 // Set to 1 to run the Image Test

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
	ray_dir.y = 2;
	ray_dir.z = -3;

	ray_dir = ray_dir.to_unitary();

	CRAB::Ray ray;

	std::vector<CRAB::Ray> ray_vec;

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
	ray.origin = ray_origin;
	ray.direction = ray_dir;
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
	ray.origin = ray_origin;
	ray.direction = ray_dir;
	colList = cone.Collide(ray);

	std::cout << "Ray direction = (" << ray.direction.x << "; " << ray.direction.y << "; " << ray.direction.z << ")" << std::endl;
	std::cout << "Cone one ray Intersections Number:" << colList.collisions.size() << "\n";
	for (int i = 0; i < colList.collisions.size(); i++)
		std::cout << "t" << i + 1 << " = (" << colList.collisions[i].pint.x << ";" << colList.collisions[i].pint.y << ";" << colList.collisions[i].pint.z << ")" << std::endl;

	//Collide more than one ray
	ray_vec.clear();
	for (int i = 0; i < 10; i++)
	{
		ray.direction.y += 0.01f;
		ray.direction = ray.direction.to_unitary();
		ray_vec.push_back(ray);
	}

	colList = cone.CollideAll(ray_vec);

	std::cout << "Cone more than one ray Intersections Number:" << colList.collisions.size() << "\n";

#endif

#if SPHERE_TEST == 1

	//Sphere
	CRAB::Vector4Df sphere_center;
	sphere_center.x = 0.0f;
	sphere_center.y = 10.0f;
	sphere_center.z = -10.0f;

	float sphere_radius = 5.0f;

	Sphere S = Sphere(sphere_center, sphere_radius);

	//Intersection
	ray.origin = ray_origin;
	ray.direction = ray_dir;
	colList = S.Collide(ray);

	std::cout << "Ray direction = (" << ray.direction.x << "; " << ray.direction.y << "; " << ray.direction.z << ")" << std::endl;
	std::cout << "Sphere one ray Intersections Number:" << colList.collisions.size() << "\n";
	for (int i = 0; i < colList.collisions.size(); i++)
		std::cout << "t" << i + 1 << " = (" << colList.collisions[i].pint.x << ";" << colList.collisions[i].pint.y << ";" << colList.collisions[i].pint.z << ")" << std::endl;

	//Collide more than one ray
	ray_vec.clear();
	for (int i = 0; i < 10; i++)
	{
		ray.direction.y += 0.01f;
		ray.direction = ray.direction.to_unitary();
		ray_vec.push_back(ray);
	}

	colList = S.CollideAll(ray_vec);

	std::cout << "Sphere more than one ray Intersections Number:" << colList.collisions.size() << "\n";

#endif

#if IMAGE_TEST == 1
	
	// SCREEN SIZE
	float screenWidth = 5.0f;
	float screenHeight = 5.0f;	
	float resolution = 10.0f;	// pixels by unit
	CRAB::Vector4Df centerPoint = { 0.0f, 0.0f, 2.0f, 1.0f };

	// CAMERA
	CRAB::Ray camera;
	camera.origin = { 0.0f, 0.0f, 4.0f, 1.0f };		// x y z w
	camera.direction = { 0.0f, 0.0f, -1.0f, 0.0f };	// to center of the screen

	// CYLINDER
	Cylinder cylinder(4.0f, 0.5f, { 0.0f, 0.0f, -10.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }); // Height Radius Center Direction

	// CONE
	Cone cone(9.0f, 2.0f, { 0.0f, 4.0f, -10.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f });

	// CUBE
	Cube cube1({ 0.0f, 0.0f, -17.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }, 5.0f);
	Cube cube2({ 0.0f, 5.0f, -17.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }, 5.0f);
	Cube cube3({ 0.0f, 10.0f, -17.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }, 5.0f);

	// QUAD
	Quad quad({ 3.5f, -3.0f, 0.0f, 1.0f }, { -3.5f, -3.0f, 0.0f, 1.0f }, { -3.5f, 0.0f, 0.0f, 1.0f }, { 3.5f, 0.0f, 0.0f, 1.0f });

	// RAY INTERSECTIONS
	std::vector<CRAB::RayCollisionList> RayTracing;
	RayTracing.clear();
	CRAB::Vector4Df pixel;
	pixel.z = centerPoint.z;
	for (int j = 0; j < screenHeight * resolution; j++)
	{
		pixel.y = centerPoint.y + (screenHeight / 2.0) - (j / resolution) - (2.0 / resolution);
		for (int i = 0; i < screenWidth * resolution; i++)
		{
			pixel.x = centerPoint.x - (screenWidth / 2.0) + (i / resolution) + (2.0 / resolution);

			camera.direction = (pixel - camera.origin).to_unitary();
			
			RayTracing.push_back(cylinder.Collide(camera));
			RayTracing.push_back(cone.Collide(camera));
			RayTracing.push_back(cube1.Collide(camera));
			RayTracing.push_back(cube2.Collide(camera));
			RayTracing.push_back(cube3.Collide(camera));
			RayTracing.push_back(quad.Collide(camera));
			
			// RENDER
			if (RayTracing[0].collisions.size() > 0) std::cout << "0";
			else if (RayTracing[1].collisions.size() > 0) std::cout << "+";
			else if (RayTracing[5].collisions.size() > 0) std::cout << "|";
			else std::cout << " ";
			RayTracing.clear();
		}
		std::cout << std::endl;
	}
	
	
	
	
	


#endif


	//start the window and the rendering
	Start_Window(argc, argv);
	std::cin >> buff;

	return 0;
}