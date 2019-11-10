#include "RayCastRender.h"
#include "GlobalVariables.h"

#include <iostream>
#include <algorithm>
#include "Quad.h"

#include <time.h>
#include<omp.h>

#define PRINT 0 //set if will print the colisions on terminal or not

//#define X_PRINT 256
//#define Y_PRINT 256

using namespace CRAB;

RayCast::RayCast(){
}

RayCast::RayCast(const CRAB::Camera &cam) : resolution(cam.resolution){

	accumulateBuffer = new(nothrow) CRAB::Vector4Df[resolution.x * resolution.y * sizeof(CRAB::Vector4Df)];

	for (unsigned y = 0, s = (unsigned)(resolution.x * resolution.y); y < s; y ++) {
		accumulateBuffer[y] = CRAB::Vector4Df{ 0.5f, 0.5f, 0.5f, 0.5f };
	}
}

RayCast::~RayCast() {
	delete[]accumulateBuffer;
}

inline bool InShadow(const Ray& ray, const std::vector<Object>& objects, const Light& light) {
	const float dist = light.LightDistance(ray.origin);
	for (const Object& obj : objects) {
		//if alfa == 1 them the material cast a full shadow
		if (obj.getMaterial()->alfa == 1) {//TODO: change this to light interaction with trasnparence
			const CRAB::Collision col = obj.Collide(ray);//Collision
			if (col.distance > 0.01f && col.distance < dist) {
				return true;
			}
		}
	}
	return false;

}

//Refract Vector
inline Vector4Df refract(const Vector4Df & incident, const Vector4Df & normal, const float & ior_t, const float & ior_i) {
	//T = eta*I + (eta*C1 - C2)*N
	//eta = etai/etat -> eta
	//C1 = cos(theta) = N.I -> n_dot_i
	//C2 = sqrt(1-eta²*(1-cos²(theta))) -> k
	
	Vector4Df Nrefr;
	float n_dot_i = dot(normal, incident); //N.I = cos(theta)
	float etai, etat, eta, k;

	if (n_dot_i > 1){
		n_dot_i = 1;
	}
	else if(n_dot_i < -1){
		n_dot_i = -1;
	}

	if (n_dot_i < 0){//Entering the surface (cos(theta)<0)
		n_dot_i = -n_dot_i;
		Nrefr = normal;
		etai = ior_i;
		etat = ior_t;
	}
	else{//Leaving the surface (Reverse Normal)
		Nrefr = normal*(-1);
		etai = ior_t;
		etat = ior_i;
	}
	eta = etai / etat; //eta
	k = 1 - (eta * eta * (1 - n_dot_i * n_dot_i));

	if (k < 0) {// Total internal reflection (There is no refraction in this case)
		return Vector4Df{0.0f, 0.0f, 0.0f, 0.0f};
	}

	return ((incident*eta) + Nrefr*(eta*n_dot_i - sqrtf(k))).to_unitary();

}

float fresnel(const Vector4Df & incident, const Vector4Df & normal, const float & ior_t, const float & ior_i) {
	float n_dot_i = dot(normal, incident); //N.I = cos(theta)
	float etai, etat, sint, kr;

	if (n_dot_i > 1) {
		n_dot_i = 1;
	}
	else if (n_dot_i < -1) {
		n_dot_i = -1;
	}

	if (n_dot_i < 0) {//Entering the surface (cos(theta)<0)
		etai = ior_i;
		etat = ior_t;
	}
	else {//Leaving the surface (Reverse Normal)
		etai = ior_t;
		etat = ior_i;
	}
	// Compute sini using Snell's law
	sint = etai / etat * sqrtf(std::max(0.0f, 1 - n_dot_i * n_dot_i));
	// Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrtf(std::max(0.f, 1 - sint * sint));
		n_dot_i = fabsf(n_dot_i);
		float Rs = ((etat * n_dot_i) - (etai * cost)) / ((etat * n_dot_i) + (etai * cost));
		float Rp = ((etai * n_dot_i) - (etat * cost)) / ((etai * n_dot_i) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2;
	}


	return kr;
}

inline Vector4Df ray_cast(const Ray &ray, const std::vector<Object> &objects, const std::vector<Light *> &lights, bool print, float near, int depth)
{
	float dist = INFINITY;
	Collision closest_collision;
	const Object *closest_obj = nullptr;
	#if PRINT == 1
	int id = 0;
	#endif
	for (const Object &obj : objects) {
		const CRAB::Collision col = obj.Collide(ray);//Collision
		const float o_dist = col.distance;
		if (o_dist < dist && o_dist > near) {
			dist = o_dist;
			closest_collision = col;
			closest_obj = &obj;
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
	Vector4Df vec_offset = closest_collision.pint + ray.direction*0.001f;
	Vector4Df accucolor = Vector4Df{ 0.1f, 0.68f, 0.93f, 0.0f };
	if (closest_obj) {
		//accucolor = Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f };
		const Material mat = *closest_obj->getMaterial();

		accucolor = mat.ka;
		//accucolor = Vector4Df{ 1,1,1,1 } * (1.0f/ closest_collision.distance) ;
		const CRAB::Vector4Df N = closest_collision.geometry->getNormal(closest_collision.pint);
		//accucolor = N ;
		for (Light * light : lights)
		{
			float dot_d_n = 0.0f;
			//const CRAB::Vector4Df L = CRAB::Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f};
			//if (typeid(*light) == typeid(DirectionalLight)){
			const CRAB::Vector4Df L = light->GetLightDirection(closest_collision.pint);
			dot_d_n = dot(L, N);
			//}

			if ((dot_d_n > 0) && !InShadow(CRAB::Ray{ closest_collision.pint, L }, objects, *light)) {
				if (light->on){
					accucolor += light->Illumination(mat,
						N, ray.direction * (-1.0f),
						closest_collision.pint)*mat.alfa;
				}
			}
		}

		
		/*
		if (depth < 1)
		{
			if (mat.reflection && mat.ior>1)
			{
				Vector4Df refractionColor = Vector4Df{ 0,0,0,0 };
				Vector4Df reflectionColor = Vector4Df{ 0,0,0,0 };

				//const Vector4Df refract_ray = refract(ray.direction, N, mat.ior, 1);
				//refractionColor = ray_cast(CRAB::Ray{ vec_offset, refract_ray }, objects, lights, false, SMALL_NUMBER, depth + 1);

				const CRAB::Vector4Df R = CRAB::reflection(ray.direction * (-1.0f), N);
				reflectionColor = ray_cast(CRAB::Ray{ closest_collision.pint, R }, objects, lights, false, SMALL_NUMBER, depth + 1) * mat.reflection;
				
				// Compute fresnel
				float kr = fresnel(ray.direction, N, mat.ior, 1);
				
				if (kr < 1) {
					const Vector4Df refract_ray = refract(ray.direction, N, mat.ior, 1);
					refractionColor = ray_cast(CRAB::Ray{ vec_offset, refract_ray }, objects, lights, false, SMALL_NUMBER, depth + 1);
				}

				accucolor += (reflectionColor * kr + refractionColor * (1 - kr));
			}
			else if (mat.reflection) {
				const CRAB::Vector4Df R = CRAB::reflection(ray.direction * (-1.0f), N);
				accucolor += ray_cast(CRAB::Ray{ closest_collision.pint, R }, objects, lights, false, SMALL_NUMBER, depth + 1) * mat.reflection;
			}


			else if (mat.ior > 1) {
				const Vector4Df refract_ray = refract(ray.direction, N, mat.ior, 1);
				accucolor += ray_cast(CRAB::Ray{ vec_offset , refract_ray }, objects, lights, false, SMALL_NUMBER, depth + 1);
			}
		}*/
		
		
		if (mat.reflection && depth < 1) {
			const CRAB::Vector4Df R = CRAB::reflection(ray.direction * (-1.0f), N);
			accucolor += ray_cast(CRAB::Ray{ closest_collision.pint, R},objects,lights,false,SMALL_NUMBER,depth+1) * mat.reflection;
		}

		
		if (mat.ior > 1 && depth < 1) {
			const Vector4Df refract_ray = refract(ray.direction, N, mat.ior, 1);
			accucolor += ray_cast(CRAB::Ray{ vec_offset , refract_ray }, objects, lights, false, SMALL_NUMBER, depth + 1)*(1-mat.alfa);
		}
		
		accucolor = accucolor * 0.5f;

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
	Object *colidiu = nullptr;
	CRAB::Collision *col = nullptr;
	float dist = INFINITY;
	Matrix4 m;
	Matrix4 m_to_origin;
	Matrix4 m_back;
	Matrix4 m_reflection;
	Vector4Df to_origin;
	Matrix4 to_cam = CRAB::ToCamera(cam);
	for (Object &obj : objects) {
		col = &obj.Collide(ray);//Collision
		const float o_dist = col->distance;
		if (o_dist < dist && o_dist > cam.n) {
			dist = o_dist;
			colidiu = &obj;

			//Abitrary Plan Reflection
			if (reflection_en == 1)
			{
				Vector4Df n = colidiu->getNormalVector(col->pint);
				m_reflection = reflectionArbitrary(n);
				m_to_origin = translate((col->pint) * -1);
				m_back = translate(col->pint);
				m = m_back * m_reflection * m_to_origin;
				//colidiu->getGeometry()->transform(m);
				

				for (Object &obj : objects) {
					Object *scene_obj = &obj;
					scene_obj->getGeometry()->transform(m);
				}
				reflection_en = 0;
			}
			

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

CRAB::Vector4Df* RayCast::Render(const CRAB::Camera &cam, const std::vector<Object> &objects, std::vector<Light*> &lights) {

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

	#pragma omp parallel for collapse(2) num_threads(16) schedule(dynamic) //default(shared)
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Vector4Df direction = posi_pix_0_0 + up * (y) + left * (-x);
			//direction.normalize();

#if PRINT == 1
			if (x == X_PRINT && y == Y_PRINT) {
				std::cout << "*** Testando *** x:" << x << " y:" << y << "\n";
				print = true;
			}else {
				print = false;
			}
#endif

			//projection
			Vector4Df origin, current_pixel;
			switch (graphicalProjection)
			{
			case 1:
				origin = cam.position;
				break;
			case 2:
				current_pixel = cam.position + direction;
				origin = current_pixel - base * cam.n;
				direction = base;
				break;
			case 3:
			{
				current_pixel = cam.position + direction;
				float tg_x = tanf((obliqueAngleX * M_PI / 180));
				float tg_y = tanf((obliqueAngleY * M_PI / 180));
				Matrix4 mm_teste2{
					1.0f, 0.0f, tg_x, 0.0f,
					0.0f, 1.0f, tg_y, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f };
				direction = mm_teste2 * (base * cam.n);
				origin = current_pixel - direction;
				break;
			}
			default:
				origin = cam.position;
				break;
			}

			direction.normalize();
			accumulateBuffer[y*width + x] = ray_cast(Ray { origin, direction }, objects, lights, print, cam.n, 0);

		}
	}
	
	t = clock() - t;	
	//std::cout << "levou " << t << " clocks ou " << ((float)t) / CLOCKS_PER_SEC << " segundos ou " << 1.0f/(((float)t) / CLOCKS_PER_SEC) << " fps\n";
	
	return accumulateBuffer;
}

const Vector4Df colors[4] = { Vector4Df{1,0,0,0},Vector4Df{0.15f, 0.87f, 0.15f},Vector4Df{0.9f,0.9f,0,0},Vector4Df{0,0.6f,1,0} };
const Vector4Df light = Vector4Df{0.6f,-0.5f,0.1f, 0.0f}.to_unitary();
const Vector4Df light_amb = Vector4Df{ 0.2f,0.2f,0.2f, 0.2f };
const Vector4Df reflectio = Vector4Df{ 0.3f,0.3f,0.3f, 0.0f };
const Vector4Df vec2 = Vector4Df{ 2.0f,2.0f,2.0f, 2.0f };
const Vector4Df sky_color = Vector4Df{ 0.8f, 0.8f, 0.8f, 1.0f };

const Quad plane = Quad(Vector4Df{ -14.0f,-1.0f,-14.0f, 1.0f }, Vector4Df{ -14.0f,-1.0f,16.0f, 1.0f },
	Vector4Df{ 16.0f,-1.0f,16.0f, 1.0f }, Vector4Df{ 16.0f,-1.0f,-14.0f, 1.0f });

__forceinline float __fastcall CollideClosestQuad(register const Vector4Df &origin, register const Vector4Df &direction,
	register const Quad &quad, Vector4Df &p_plane) {

	//const Vector4Df &normal = cross_simd(_mm_castsi128_ps(_mm_stream_load_si128((__m128i*)&(quad.e1))),
	//	_mm_castsi128_ps(_mm_stream_load_si128((__m128i*)&(quad.e2))));
	const Vector4Df &normal = cross_simd(quad.e1, quad.e2);


	const Vector4Df &dir = dot_simd_Vec(direction, normal);
	if (_mm_ucomigt_ss(dir, _mm_setzero_ps()))return INFINITY;
	const Vector4Df &t = dot_simd_Vec(quad.v1 - origin, normal) / dir;

	//const Vector4Df &p_plane = ray.origin + ray.direction*t;
	p_plane = *(Vector4Df*)&_mm_fmadd_ps(direction, t, origin);


	const float proj2 = dot_simd(p_plane - quad.v1, quad.e2);
	const float proj1 = dot_simd(p_plane - quad.v1, quad.e1);

	//const bool hit = proj1 >= 0.000001f && proj1 <= quad.size1 && proj2 >= 0.000001f && proj2 <= quad.size2;
	//if (hit) {
	if (proj1 >= 0.000001f && proj2 >= 0.000001f && proj1 <= quad.size1 && proj2 <= quad.size2) {
		//if (proj1 >= 0.000001f && proj2 >= 0.000001f && proj2 + proj1 <= (quad.size2 + quad.size1)*0.5f) {//to be a triangle
		return _mm_cvtss_f32(t);
	}
	return INFINITY;
}

inline Vector4Df __fastcall ray_cast(register const Vector4Df &origin, register const Vector4Df &direction, const std::vector<Sphere> &spheres, int recs)
{
	if (!recs)return Vector4Df{ 0.0f,0.0f,0.0f,0.0f };
	Vector4Df p_colision = {0,0,0,0};
	float p_dist = CollideClosestQuad(origin, direction, plane, p_colision);
	float dist = INFINITY, p_dist2 = INFINITY;

	Vector4Df accucolor = sky_color;

	const Sphere *collide_sphere = nullptr;

	for (int i = 0, size = spheres.size(); i < size; i+=4) {
		const Sphere &s = spheres.at(i);
		const Sphere &s2 = spheres.at(i+1);
		const Sphere &s3 = spheres.at(i+2);
		const Sphere &s4 = spheres.at(i+3);

		
		const CRAB::Vector4Df W = origin - s.center;		// The difference of P0 - C
		const CRAB::Vector4Df W2 = origin - s2.center;		// The difference of P0 - C
		const CRAB::Vector4Df W3 = origin - s3.center;		// The difference of P0 - C
		const CRAB::Vector4Df W4 = origin - s4.center;		// The difference of P0 - C

		// Coefficients of the equation
		//const float A = 1.0f;//considering that the direction is already normalized
		
		const Vector4Df B{ dot_simd(W, direction),dot_simd(W2, direction),dot_simd(W3, direction),dot_simd(W4, direction) };
		const Vector4Df C = Vector4Df{ dot_simd(W, W),dot_simd(W2, W2),dot_simd(W3, W3),dot_simd(W4, W4) } - Vector4Df{ s.radius, s2.radius, s3.radius, s4.radius };
		// Discriminant	
		const Vector4Df Delta = (B*B - C);

		//if (_mm_test_all_ones(_mm_cvtps_epi32(_mm_cmplt_ps(Delta, _mm_setzero_ps()))))continue;
		const __m128 x = _mm_cmpgt_ps(Delta, _mm_setzero_ps());
		if (_mm_testz_ps(x, x))continue;

		float o_dist = INFINITY;
		int j = 10;
		const Vector4Df &dists = ((Vector4Df{ 0.0f,0.0f,0.0f,0.0f }-B) - *(Vector4Df*)&_mm_sqrt_ps(Delta));
		o_dist = dists.x;
		if (o_dist < dist && o_dist > 0.0001f) { dist = o_dist; accucolor = colors[0]; collide_sphere = &s; }
		o_dist = _mm_cvtss_f32(_mm_shuffle_ps(dists, dists, _MM_SHUFFLE(0, 0, 0, 1)));
		if (o_dist < dist && o_dist > 0.0001f) { dist = o_dist; accucolor = colors[1]; collide_sphere = &s2;}
		o_dist = _mm_cvtss_f32(_mm_shuffle_ps(dists, dists, _MM_SHUFFLE(0, 0, 0, 2)));
		if (o_dist < dist && o_dist > 0.0001f) { dist = o_dist; accucolor = colors[2]; collide_sphere = &s3;}
		o_dist = _mm_cvtss_f32(_mm_shuffle_ps(dists, dists, _MM_SHUFFLE(0, 0, 0, 3)));
		if (o_dist < dist && o_dist > 0.0001f) { dist = o_dist; accucolor = colors[3]; collide_sphere = &s4;}
	}
	if (dist == INFINITY && p_dist == INFINITY)return accucolor;
	if(recs == 1)return Vector4Df{ 0.0f,0.0f,0.0f,0.0f };
	

	Vector4Df n;
	if (dist < p_dist) {
		p_colision = *(Vector4Df*)&_mm_fmadd_ps(direction, _mm_set_ps1(dist), origin);
		n = (collide_sphere->center - p_colision);
	}
	else {
		n = cross_simd(plane.e2, plane.e1);
		accucolor = Vector4Df{ 0.6f,0.6f,0.6f,0 };
	}
	n.normalize();


	Vector4Df final_color = accucolor * light_amb;
	

	const Vector4Df luz_col = ray_cast(p_colision, light*(-1.0f), spheres, 1);

	const Vector4Df reflec_dir = (vec2*(n*dot_simd_Vec(n, direction)) - direction);

	
	if (luz_col == sky_color) {
		const Vector4Df l_dir = dot_simd_Vec(n, light);
		//diffuse
		if (_mm_ucomigt_ss(l_dir, _mm_setzero_ps())) {
			final_color = *(Vector4Df*)&_mm_fmadd_ps(l_dir, accucolor, final_color);

			
		}
		//specular
		Vector4Df spec = dot_simd_Vec(reflec_dir, light);
		if (_mm_ucomigt_ss(spec, _mm_setzero_ps())) {
			spec = spec * spec;
			spec = spec * spec;
			spec = spec * spec;
			spec = spec * spec;
			final_color = *(Vector4Df*)&_mm_fmadd_ps(spec, accucolor, final_color);
			
		}
		
	
		
	}

	const Vector4Df reflec = ray_cast(p_colision, reflec_dir*(-1.0f), spheres, recs - 1);
	//if(reflec != Vector4Df{ 0.8f, 0.8f, 0.8f, 0.0f }){
	if (dist < p_dist) {
		final_color = *(Vector4Df*)&_mm_fmadd_ps(reflectio, reflec, final_color);
	}
	else {
		final_color = *(Vector4Df*)&_mm_fmadd_ps(Vector4Df{ 0.3f,0.3f,0.3f,0.0f }, reflec, final_color);
	}
	//}

	return final_color*sky_color;
}

CRAB::Vector4Df* RayCast::Render(const CRAB::Camera &cam, const std::vector<Sphere> &spheres)
{
	
	//clock_t t;
	//t = clock();

	const Vector4Df base = (cam.view - cam.position).to_unitary();
	const Vector4Df up = cam.up * (cam.dimensions.y / cam.resolution.y);
	const Vector4Df left = cross_simd(cam.up, base) * (cam.dimensions.x / cam.resolution.x);


	//Vector4Df *pix = accumulatebuffer;
	const int width = (int)cam.resolution.x;
	const int height = (int)cam.resolution.y;

	bool print = false;

	const Vector4Df posi_pix_0_0 = base * cam.n + up * (height*(-0.5f) + 0.5f) + left * (width*(0.5f) - 0.5f);

//#pragma omp parallel for num_threads(16) schedule(guided)
#pragma omp parallel for num_threads(16) schedule(dynamic, 1)
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Vector4Df direction = posi_pix_0_0 + up * (y)+left * (-x);
			direction.normalize();

			accumulateBuffer[y*width + x] = ray_cast(cam.position, direction, spheres, 4);

		}
	}

	//t = clock() - t;
	//std::cout << "levou " << t << " clocks ou " << ((float)t) / CLOCKS_PER_SEC << " segundos ou " << 1.0f/(((float)t) / CLOCKS_PER_SEC) << " fps\n";

	return accumulateBuffer;
}