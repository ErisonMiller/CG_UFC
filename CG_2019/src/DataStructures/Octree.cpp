#include "Octree.h"

using namespace CRAB;


#define MAX_TREE_SIZE 64
const CRAB::Vector4Df cube_points[8] = {
	CRAB::Vector4Df{-0.5774,-0.5774, 0.5774,0.0f},
	CRAB::Vector4Df{ 0.5774,-0.5774, 0.5774,0.0f},
	CRAB::Vector4Df{ 0.5774, 0.5774, 0.5774,0.0f},
	CRAB::Vector4Df{-0.5774, 0.5774, 0.5774,0.0f},

	CRAB::Vector4Df{-0.5774,-0.5774,-0.5774,0.0f},
	CRAB::Vector4Df{ 0.5774,-0.5774,-0.5774,0.0f},
	CRAB::Vector4Df{ 0.5774, 0.5774,-0.5774,0.0f},
	CRAB::Vector4Df{-0.5774, 0.5774,-0.5774,0.0f}
};


TreeElement* PovoateTree(const FaceList& fatherFaceList, const CRAB::Vector4Df &center, float r) {
	
	FaceList faceList;
	
	for (const Face& f : fatherFaceList) {
		if ((f.v1 - center).lengthsq() <= r*r ||
			(f.v2 - center).lengthsq() <= r*r ||
			(f.v3 - center).lengthsq() <= r*r) {
			faceList.push_back(f);
			continue;
		}

		CRAB::Vector4Df v0v1 = f.v2 - f.v1;
		CRAB::Vector4Df v0v2 = f.v3 - f.v1;
		CRAB::Vector4Df dir = cross_simd(v0v1, v0v2).to_unitary();
		CRAB::Vector4Df pvec = cross_simd(dir, v0v2);
		float det = dot_simd(v0v1, pvec);

		// ray and triangle are parallel if det is close to 0
		//if (fabs(det) < 0.0001) return false;

		float invDet = 1.0f / det;

		CRAB::Vector4Df tvec = center - f.v1;
		float u = dot_simd(tvec, pvec) * invDet;
		if (u < 0 || u >= 1) continue;

		CRAB::Vector4Df qvec = cross_simd(tvec, v0v1);
		float v = dot_simd(dir, qvec) * invDet;
		if (v <= 0 || u + v > 1) continue;

		float t = CRAB::dot(v0v2, qvec) * invDet;
		if (t < -r || t > r) continue;

		faceList.push_back(f);
		//return true;

		//Vector4Df e1 = f.v2 - f.v1;
		//Vector4Df e2 = f.v3 - f.v1;
		//Vector4Df normal = cross_simd(e1, e2).to_unitary();
		//float t = dot(f.v1 - center, normal);
		//if (t > r) continue;
		//if()
		//if ((f.v1 - center).lengthsq() <= r2 ||
		//	(f.v2 - center).lengthsq() <= r2 ||
		//	(f.v3 - center).lengthsq() <= r2) {
		//	faceList.push_back(f);
		//}
	}


	if(faceList.empty()) return new TreeElement(TREE_EMPTY_NODE);
	
	
	if (faceList.size() > MAX_TREE_SIZE)
	{
		r *= 0.5f;
		OcElementNode* ocn = new OcElementNode();
		for (int i = 0; i < 8; i++) {
			ocn->sons[i] = PovoateTree(faceList, center + cube_points[i] * r, r);
		}
		return ocn;
	}
	
	return new OcElementLeaf(faceList);
	
	//if(faceList.empty())return new TreeElement(TREE_EMPTY_NODE);
	//if (faceList.size() > MAX_TREE_SIZE){
	//
	//	FaceList fs[8];
	//	for (const Face& f : faceList) {
	//		bool cx = insidePlane(f, center, Vector4Df{ -1,0,0,0 });
	//		bool cy = insidePlane(f, center, Vector4Df{ 0,-1,0,0 });
	//		bool cz = insidePlane(f, center, Vector4Df{ 0,0,1,0 });
	//
	//		if (cx) {//left
	//			if (cy) {//down
	//				if (cz)	fs[0].push_back(f);
	//				else	fs[4].push_back(f);
	//			}
	//			else {//up
	//				if (cz)	fs[3].push_back(f);
	//				else	fs[7].push_back(f);
	//			}
	//		}
	//		else {//right
	//			if (cy) {//down
	//				if (cz)	fs[1].push_back(f);
	//				else	fs[5].push_back(f);
	//			}
	//			else {//up
	//				if (cz)	fs[2].push_back(f);
	//				else	fs[6].push_back(f);
	//			}
	//		}
	//	}
	//
	//	r *= 0.5f;
	//	OcElementNode* ocn = new OcElementNode();
	//	for (int i = 0; i < 8; i++) {
	//		ocn->sons[i] = PovoateTree(fs[i], center + cube_points[i] * r, r);
	//	}
	//	return ocn;
	//}
	//return new OcElementLeaf(faceList);
	
}


OcTree::OcTree(const FaceList &faceList) {

	CRAB::Vector4Df min{ INFINITY,INFINITY,INFINITY ,INFINITY };
	CRAB::Vector4Df max{ -INFINITY,-INFINITY,-INFINITY,-INFINITY };
	for (const Face& f : faceList) {
		min = CRAB::min(min, f.v1);
		max = CRAB::max(max, f.v1);

		min = CRAB::min(min, f.v2);
		max = CRAB::max(max, f.v2);

		min = CRAB::min(min, f.v3);
		max = CRAB::max(max, f.v3);
	}
	
	Vector4Df ax = (max - min);
	
	r = fmaxf(ax.x, fmaxf(ax.y, ax.z));
	center = min + Vector4Df{ 0.5f,0.5f,0.5f,0 }*r;
	r = (center - min).length();

	tree = PovoateTree(faceList, center, r);
}



Collision TreeCollision(TreeElement* t, const CRAB::Vector4Df& center, float r, const CRAB::Ray& ray) {
	
	Collision col{ INFINITY };
	if (t->state == TREE_EMPTY_NODE) return col;
	
	const Vector4Df& n = cross_simd(center, ray.direction);
	if (dot_simd(n,n) > r*r)return col;//don't collide


	if (t->state == TREE_SEMI_NODE)
	{
		OcElementNode* ocn = (OcElementNode*)t;
		r *= 0.5f;
		for (int i = 0; i < 8; i++) {
			Collision col2 = TreeCollision(ocn->sons[i], center + cube_points[i] * r, r, ray);
			if (col2.distance < col.distance)col = col2;
		}
		return col;
	}

	OcElementLeaf* ocl = (OcElementLeaf*)t;
	for (Triangle& t : ocl->faces) {
		const Collision c1 = t.CollideClosest(ray);
		if (c1.distance < col.distance)col = c1;
	}
	return col;
}


CRAB::Collision OcTree::CollideClosest(const CRAB::Ray& ray) {
	return TreeCollision(tree, center - ray.origin, r, ray);
}
