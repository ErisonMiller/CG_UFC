#include "RayCastRender.h"


#include <iostream>

using namespace CRAB;

RayCast::RayCast(const CRAB::Camera &cam) : resolution(cam.resolution){
    accumulateBuffer = (CRAB::Vector4Df*)malloc(resolution.x * resolution.y * sizeof(CRAB::Vector4Df));

	for (unsigned y = 0, s = (unsigned)(resolution.x * resolution.y); y < s; y ++) {
		accumulateBuffer[y] = CRAB::Vector4Df{ 0.5f, 0.5f, 0.5f, 0.5f };
	}
}


inline Vector4Df ray_cast(const Ray &ray, std::vector<Object> &objects, bool print)
{
	float dist = INFINITY;

	Vector4Df accucolor = Vector4Df{ 0.0f, 0.0f, 1.0f, 0.0f };
	

	int id = 0;
	for (Object &obj : objects) {
		
		const float o_dist = obj.Collide(ray);
		if (o_dist < dist) {
			dist = o_dist;
			accucolor = obj.getColor();
		}
		if (print) {
			RayCollisionList cols = obj.CollideAll(ray);
			std::cout << "-- Colisoes com :" << id << "\n";
			for (Collision c : cols.collisions) {
				std::cout << "    t :" << c.distance << "; ";
				std::cout << "    p :" << c.pint.x << " " << c.pint.y << " " << c.pint.z << "\n";
			}
		}
		id++;
	}

	return accucolor;
}


CRAB::Vector4Df* RayCast::Render(const CRAB::Camera &cam, std::vector<Object> &objects) {
	
	const Vector4Df base = (cam.view - cam.position).to_unitary();
	const Vector4Df up = cam.up * (cam.dimensions.y / cam.resolution.y);
	const Vector4Df left = cross(cam.up, base) * (cam.dimensions.x / cam.resolution.x);


	//Vector4Df *pix = accumulatebuffer;
	const int width = (int)cam.resolution.x;
	const int height = (int)cam.resolution.y;

	bool print = false;

	#pragma omp parallel for
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Vector4Df direct = base * cam.n;
			direct += up * (height*(-0.5f) + y + 0.5f);
			direct += left * (width*(0.5f) - x - 0.5f);
			direct.normalize();

			//if (x == 256 && y == 256) {
			//	std::cout << "*** Testando *** x:" << x << " y:" << y << "\n";
			//	print = true;
			//}else {
			//	print = false;
			//}

			accumulateBuffer[y*width + x] = ray_cast(Ray{ cam.position, direct }, objects, print);

		}
	}


    //RayPathRender(triangles, cam, accumulatebuffer);
	return accumulateBuffer;
}