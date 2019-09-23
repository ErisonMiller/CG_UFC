#include "Octree.h"

using namespace CRAB;

Octree::Octree()
{
}


Octree::~Octree()
{
}


Ocject::Ocject()
{

}




Vector4Df min3(const Vector4Df &v1, const Vector4Df &v2) {
	return Vector4Df{ v1.x < v2.x ? v1.x : v2.x, v1.y < v2.y ? v1.y : v2.y, v1.z < v2.z ? v1.z : v2.z };
}
Vector4Df max3(const Vector4Df &v1, const Vector4Df &v2) {
	return Vector4Df{ v1.x > v2.x ? v1.x : v2.x, v1.y > v2.y ? v1.y : v2.y, v1.z > v2.z ? v1.z : v2.z };
}

Ocject::Ocject(std::vector<Face> &fs)
{
	Vector4Df mx = Vector4Df{ -HUGE_VALF,-HUGE_VALF,-HUGE_VALF }, mn = Vector4Df{ HUGE_VALF,HUGE_VALF,HUGE_VALF };
	center = Vector4Df{ 0,0,0 };
	for (const Face &f : fs) {
		mx = max3(mx, f.v1);
		mx = max3(mx, f.v2);
		mx = max3(mx, f.v3);

		mn = min3(mn, f.v1);
		mn = min3(mn, f.v2);
		mn = min3(mn, f.v3);
	}
	min = mn; max = mx;
	center = (max + min)*0.5f;
	r = (max - center).length();
	sons = fs.size();

	if (sons > 16) {
		Subdivide(fs);
		//if (filhos.size() > 0) {
		//	faces.clear();
		//}
	}
	//if(filhos.size == 0) {
	//	for (const Face &f : fs) {
	//		triangles.push_back(Triangle(f.v1, f.v2, f.v3));
	//	}
	//}
}

//Ocject::Ocject(std::vector<Triangle> &fs, const CRAB::Vector4Df &cmin, const CRAB::Vector4Df &cmax, const CRAB::Vector4Df &ccenter) : faces(fs),
//min(cmin), max(cmax), center(ccenter)
//{
//	r = (max - center).length();
//	sons = fs.size();
//}

int Classify(Face f, Vector4Df v_plane, Vector4Df direction) {
	int inside = 0;
	if (dot_simd(f.v1 - v_plane, direction) >= 0)inside++;
	if (dot_simd(f.v2 - v_plane, direction) >= 0)inside++;
	if (dot_simd(f.v3 - v_plane, direction) >= 0)inside++;
	return inside;
}

inline void Ocject::Subdivide(std::vector<Face> &faces)
{
	std::vector<Face> f1, f2, f3, f4, f5, f6, f7, f8;
	for (Face &f : faces) {
		int cx, cy, cz;
		cx = Classify(f, center, Vector4Df{ 1,0,0,0 });
		cy = Classify(f, center, Vector4Df{ 0,1,0,0 });
		cz = Classify(f, center, Vector4Df{ 0,0,1,0 });

		if (cx < 3) {//left
			if (cy < 3) {//down
				if (cz > 0)f1.emplace_back(f);
				if (cz < 3)f5.emplace_back(f);
			}
			if (cy > 0) {//up
				if (cz > 0)f4.emplace_back(f);
				if (cz < 3)f8.emplace_back(f);
			}
		}
		if(cz > 0) {//right
			if (cy < 3) {//down
				if (cz > 0)f2.emplace_back(f);
				if (cz < 3)f6.emplace_back(f);
			}
			if (cy > 0) {//up
				if (cz > 0)f3.emplace_back(f);
				if (cz < 3)f7.emplace_back(f);
			}
		}
	}
	if (f1.size() > 0 && f1.size() < sons)filhos.push_back(new Ocject(f1));
	if (f2.size() > 0 && f2.size() < sons)filhos.push_back(new Ocject(f2));
	if (f3.size() > 0 && f3.size() < sons)filhos.push_back(new Ocject(f3));
	if (f4.size() > 0 && f4.size() < sons)filhos.push_back(new Ocject(f4));
	if (f5.size() > 0 && f5.size() < sons)filhos.push_back(new Ocject(f5));
	if (f6.size() > 0 && f6.size() < sons)filhos.push_back(new Ocject(f6));
	if (f7.size() > 0 && f7.size() < sons)filhos.push_back(new Ocject(f7));
	if (f8.size() > 0 && f8.size() < sons)filhos.push_back(new Ocject(f8));
}
