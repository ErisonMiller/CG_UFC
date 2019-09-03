#include "RayCastRender.h"


#include <iostream>

#include <time.h>
#include<omp.h>

#define PRINT 0 //set if will print the colisions on terminal or not

//#define X_PRINT 256
//#define Y_PRINT 256

using namespace CRAB;

RayCast::RayCast(const CRAB::Camera &cam) : resolution(cam.resolution){
    accumulateBuffer = (CRAB::Vector4Df*)malloc(resolution.x * resolution.y * sizeof(CRAB::Vector4Df));

	for (unsigned y = 0, s = (unsigned)(resolution.x * resolution.y); y < s; y ++) {
		accumulateBuffer[y] = CRAB::Vector4Df{ 0.5f, 0.5f, 0.5f, 0.5f };
	}
}


__forceinline Vector4Df ray_cast(register const Ray &ray, const std::vector<Object> &objects, bool print, float cam_near)
{
	float dist = INFINITY;

	Vector4Df accucolor = Vector4Df{ 0.0f, 0.0f, 1.0f, 0.0f };
	
	#if PRINT == 1
	int id = 0;
	#endif
	for (const Object &obj : objects) {		
		const float o_dist = obj.Collide(ray);
		if (o_dist < dist && o_dist > cam_near) {
			//TODO remove this visible after
			if (obj.visible) { dist = o_dist; accucolor = obj.color; }
		}
		#if PRINT == 1
		if (print) {
			RayCollisionList cols = obj.CollideAll(ray);
			std::cout << "-- Colisoes com :" << id << " " << typeid(*obj.getGeometry()).name() << "\n";
			for (Collision c : cols.collisions) {
				std::cout << "    t :" << c.distance << "; ";
				std::cout << "    p :" << c.pint.x << " " << c.pint.y << " " << c.pint.z << "\n";
			}
		}
		id++;
		#endif
	}

	return accucolor;
}

//will be removed after
Object* RayCast::RayPick(const CRAB::Camera &cam, std::vector<Object> &objects, int x, int y) {
	const Vector4Df base = (cam.view - cam.position).to_unitary();
	const Vector4Df up = cam.up * (cam.dimensions.y / cam.resolution.y);
	const Vector4Df left = cross_simd(cam.up, base) * (cam.dimensions.x / cam.resolution.x);

	const int width = (int)cam.resolution.x;
	const int height = (int)cam.resolution.y;

	bool print = false;

	const Vector4Df posi_pix_0_0 = base * cam.n + up * (height*(0.5f) - 0.5f) + left * (width*(0.5f) - 0.5f);
	Vector4Df direction = posi_pix_0_0 + up * (-y)+left * (-x);
	direction.normalize();

	std::cout << "*** Testando *** x:" << x << " y:" << y << "\n";
	
	Ray ray{ cam.position, direction };
	
	int id = 0;
	Object *colidiu = NULL;
	float dist = INFINITY;
	Matrix4 to_cam = CRAB::ToCamera(cam);
	for (Object &obj : objects) {
		const float o_dist = obj.Collide(ray);
		if (o_dist < dist && o_dist > cam.n) {
			dist = o_dist;
			colidiu = &obj;
		}
		RayCollisionList cols = obj.CollideAll(ray);
		//std::cout << "-- Colisoes com :" << id << " " << typeid(*obj.getGeometry()).name() << "\n";
		std::cout << "-- Colisoes com: " << obj.getName() << " (" << typeid(*obj.getGeometry()).name() << ")\n";
		for (Collision c : cols.collisions) {
			c.pint = to_cam * c.pint;
			std::cout << "    t :" << c.distance << "; ";
			std::cout << "    p :" << c.pint.x << " " << c.pint.y << " " << c.pint.z << "\n";
		}
		id++;
	}
	return colidiu;

}

CRAB::Vector4Df* RayCast::Render(const CRAB::Camera &cam, const std::vector<Object> &objects) {
	clock_t t;
	t = clock();

	const Vector4Df base = (cam.view - cam.position).to_unitary();
	const Vector4Df up = cam.up * (cam.dimensions.y / cam.resolution.y);
	const Vector4Df left = cross_simd(cam.up, base) * (cam.dimensions.x / cam.resolution.x);


	//Vector4Df *pix = accumulatebuffer;
	const int width = (int)cam.resolution.x;
	const int height = (int)cam.resolution.y;

	bool print = false;
	
	const Vector4Df posi_pix_0_0 = base * cam.n + up * (height*(-0.5f) + 0.5f) + left * (width*(0.5f) - 0.5f);

	#pragma omp parallel for num_threads(16) schedule(guided)
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Vector4Df direction = posi_pix_0_0 + up * (y) + left * (-x);
			direction.normalize();

#if PRINT == 1
			if (x == X_PRINT && y == Y_PRINT) {
				std::cout << "*** Testando *** x:" << x << " y:" << y << "\n";
				print = true;
			}else {
				print = false;
			}
#endif
			
			accumulateBuffer[y*width + x] = ray_cast(Ray { cam.position, direction }, objects, print, cam.n);

		}
	}

	t = clock() - t;
	std::cout << "levou " << t << " clocks ou " << ((float)t) / CLOCKS_PER_SEC << " segundos ou " << 1.0f/(((float)t) / CLOCKS_PER_SEC) << " fps\n";


    return accumulateBuffer;
}