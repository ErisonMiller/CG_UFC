//oct continuos

#include "Octree.h"

#include <iostream>

using namespace CRAB;


#define MAX_TREE_SIZE 64
#define ONE_OVER_SQRT3 0.577350269189625764509148780f

const CRAB::Vector4Df cube_points[8] = {
	CRAB::Vector4Df{-ONE_OVER_SQRT3,-ONE_OVER_SQRT3, ONE_OVER_SQRT3,0.0f},
	CRAB::Vector4Df{ ONE_OVER_SQRT3,-ONE_OVER_SQRT3, ONE_OVER_SQRT3,0.0f},
	CRAB::Vector4Df{ ONE_OVER_SQRT3, ONE_OVER_SQRT3, ONE_OVER_SQRT3,0.0f},
	CRAB::Vector4Df{-ONE_OVER_SQRT3, ONE_OVER_SQRT3, ONE_OVER_SQRT3,0.0f},

	CRAB::Vector4Df{-ONE_OVER_SQRT3,-ONE_OVER_SQRT3,-ONE_OVER_SQRT3,0.0f},
	CRAB::Vector4Df{ ONE_OVER_SQRT3,-ONE_OVER_SQRT3,-ONE_OVER_SQRT3,0.0f},
	CRAB::Vector4Df{ ONE_OVER_SQRT3, ONE_OVER_SQRT3,-ONE_OVER_SQRT3,0.0f},
	CRAB::Vector4Df{-ONE_OVER_SQRT3, ONE_OVER_SQRT3,-ONE_OVER_SQRT3,0.0f}
};

bool Line_Sphere(const Vector4Df& o, const Vector4Df& d, const Vector4Df& c, float r) {
	const CRAB::Vector4Df W = o - c;		// The difference of P0 - C

	float A = dot(d, d);
	float B = 2 * dot(W, d);
	float C = dot(W, W) - (r * r);
	// Discriminant
	float Delta = sqrtf((B * B) - (4 * A * C));
	float d1 = (-B - Delta) / (2 * A);
	float d2 = (-B + Delta) / (2 * A);
	if ((d1 > 0 && d1 < 1) || (d2 > 0 && d2 < 1)) {
		return true;
	}
	return false;

}

/*see in what side of the plane the face are in
if 0 is all on left;
if 3 is all on rigth
if 1 or 2 he is on both*/
int Posi(const Face& f, const CRAB::Vector4Df& center, int i) {
	int vs = 0;
	if (f.v1._v[i] > center._v[i])vs++;
	if (f.v2._v[i] > center._v[i])vs++;
	if (f.v3._v[i] > center._v[i])vs++;
	return vs;
}

TreeElement* PovoateTree(const FaceList& fatherFaceList, CRAB::Vector4Df min, CRAB::Vector4Df max, int depth) {
	
	if (fatherFaceList.empty()) { 
		TreeElement *t = new TreeElement(TREE_EMPTY_NODE);
		t->max = max;
		t->min = min;
		return t;
	}
	CRAB::Vector4Df _min0 = min, _max0 = max;
	if (depth) {

		for (int i = 0, s = fatherFaceList.size(); i < s; i += 2) {
			const Face& f = fatherFaceList[i];
			_min0 = CRAB::min(_min0, f.v1);
			_max0 = CRAB::max(_max0, f.v1);

			_min0 = CRAB::min(_min0, f.v2);
			_max0 = CRAB::max(_max0, f.v2);

			_min0 = CRAB::min(_min0, f.v3);
			_max0 = CRAB::max(_max0, f.v3);
		}
		_min0 = CRAB::max(min, _min0);
		_max0 = CRAB::min(max, _max0);
	}

	if (depth > 12 || fatherFaceList.size() <= MAX_TREE_SIZE) {
		TreeElement* t = new OcElementLeaf(fatherFaceList);
		t->min = _min0;
		t->max = _max0;
		return t;
	}

	FaceList faceList[8];
	for (int i = 0; i < 8; i++)
		faceList[i].reserve(fatherFaceList.size() * 0.125);
	
	CRAB::Vector4Df facesCenter = ((_max0 + _min0) * 0.5f);
	//CRAB::Vector4Df facesCenter{0,0,0,0};
	//for (int i = 0, s = fatherFaceList.size(); i < s; i += 2) {
	//	facesCenter = facesCenter + fatherFaceList[i].getCenter();
	//}
	//facesCenter = facesCenter / (fatherFaceList.size() * 0.5f);
	
	for (int i = 0, s = fatherFaceList.size(); i < s; i += 2) {
		const Face& f = fatherFaceList[i];
		int sideX = Posi(f, facesCenter, 0);
		int sideY = Posi(f, facesCenter, 1);
		int sideZ = Posi(f, facesCenter, 2);

		if (sideZ > 0) {
			if (sideY > 0) {
				if (sideX > 0) {
					faceList[6].push_back(f);
					faceList[6].push_back(fatherFaceList[i + 1]);
				}
				if (sideX < 3) {
					faceList[7].push_back(f);
					faceList[7].push_back(fatherFaceList[i + 1]);
				}
			}
			if (sideY < 3) {
				if (sideX > 0) {
					faceList[5].push_back(f);
					faceList[5].push_back(fatherFaceList[i + 1]);
				}
				if (sideX < 3) {
					faceList[4].push_back(f);
					faceList[4].push_back(fatherFaceList[i + 1]);
				}
			}

		}
		if(sideZ < 3) {
			if (sideY > 0) {
				if (sideX > 0) {
					faceList[2].push_back(f);
					faceList[2].push_back(fatherFaceList[i + 1]);
				}
				if (sideX < 3) {
					faceList[3].push_back(f);
					faceList[3].push_back(fatherFaceList[i + 1]);
				}
			}
			if (sideY < 3) {
				if (sideX > 0) {
					faceList[1].push_back(f);
					faceList[1].push_back(fatherFaceList[i + 1]);
				}
				if (sideX < 3) {
					faceList[0].push_back(f);
					faceList[0].push_back(fatherFaceList[i + 1]);
				}
			}
		}
	}
	
	
	
	//const float cut_size = 1.0f;

	OcElementNode* ocn = new OcElementNode();
	//if (depth) {
	//	for (int i = 0; i < 8; i++) {
	//		ocn->sons[i] = PovoateTree(faceList, center + cube_points[i] * r, r, depth + 1);
	//	}
	//}
	//else {
	//#pragma omp parallel for schedule(dynamic)
	//	for (int i = 0; i < 8; i++) {
	//		ocn->sons[i] = PovoateTree(faceList, center + cube_points[i] * r, r, depth + 1);
	//	}
	//}
	min = _min0;
	max = _max0;
	
	CRAB::Vector4Df _min = min, _max = facesCenter;
	ocn->sons[0] = PovoateTree(faceList[0], _min, _max, depth + 1);
	_min.x = _max.x;
	_max.x = max.x;
	ocn->sons[1] = PovoateTree(faceList[1], _min, _max, depth + 1);
	_min.y = _max.y;
	_max.y = max.y;
	ocn->sons[2] = PovoateTree(faceList[2], _min, _max, depth + 1);
	_min.x = min.x;
	_max.x = facesCenter.x;
	ocn->sons[3] = PovoateTree(faceList[3], _min, _max, depth + 1);

	_min = min;
	_min.z = facesCenter.z;
	_max = facesCenter;
	_max.z = max.z;
	ocn->sons[4] = PovoateTree(faceList[4], _min, _max, depth + 1);
	_min.x = _max.x;
	_max.x = max.x;
	ocn->sons[5] = PovoateTree(faceList[5], _min, _max, depth + 1);
	_min.y = _max.y;
	_max.y = max.y;
	ocn->sons[6] = PovoateTree(faceList[6], _min, _max, depth + 1);
	_min.x = min.x;
	_max.x = facesCenter.x;
	ocn->sons[7] = PovoateTree(faceList[7], _min, _max, depth + 1);

	int equals = 0;
	int emp = 0;
	for (int i = 0; i < 8; i++) {
		if (ocn->sons[i]->state == TREE_FULL_NODE) {
			OcElementLeaf* ocl = (OcElementLeaf*)ocn->sons[i];
			if (ocl->faces.size() == fatherFaceList.size()) {
				equals++;
			}
		}
		if (ocn->sons[i]->state == TREE_EMPTY_NODE) {
			emp++;
		}
	}
	if (equals == 8) {
		ocn->Free();
		delete ocn;
		return new OcElementLeaf(fatherFaceList);
	}
	if (emp == 8) {

		ocn->Free();
		delete ocn;
		return new TreeElement(TREE_EMPTY_NODE);
	}

	_min = CRAB::Vector4Df{ INFINITY,INFINITY ,INFINITY ,INFINITY };
	_max = CRAB::Vector4Df{ -INFINITY,-INFINITY ,-INFINITY ,-INFINITY };;
	for (int i = 0; i < 8; i++) {
		if (ocn->sons[i]->state != TREE_EMPTY_NODE) {
			_min = CRAB::min(ocn->sons[i]->min, _min);
			_max = CRAB::max(ocn->sons[i]->max, _max);
		}
	}
	ocn->min = _min;
	ocn->max = _max;
	return ocn;
	
}


int Count_Nodes_Leafs(TreeElement* t, int& nodes, int& leafs, int& max_size, int depth) {
	if (t->state == TREE_FULL_NODE) {
		OcElementLeaf* ocn = (OcElementLeaf*)t;
		if (ocn->faces.size() > max_size) {
			max_size = ocn->faces.size();
		}
		leafs++;
		return depth;
	}
	if (t->state == TREE_SEMI_NODE) {
		OcElementNode* ocn = (OcElementNode*)t;
		int d = 0, max_d = 0;
		for (int i = 0; i < 8; i++) {
			d = Count_Nodes_Leafs(ocn->sons[i], nodes, leafs, max_size, depth + 1);
			if (d > max_d) {
				max_d = d;
			}
		}
		nodes++;
		return max_d;
	}

}

TreeElement* MakeCacheFriendlyTree(TreeElement* t, int& node, int& leaf, OcElementNodeCache* nodes_array,
	OcElementLeaf* leafs_array, const CRAB::Vector4Df& center, float r) {

	if (t->state == TREE_FULL_NODE) {
		//return t;
		int posi = leaf;
		leafs_array[leaf] = *(OcElementLeaf*)t;

		//leafs_array[leaf].min = center - CRAB::Vector4Df{ 1,1,1,0 }*r;
		//leafs_array[leaf].max = center + CRAB::Vector4Df{ 1,1,1,0 }*r;

		//leafs_array[leaf].min = center;
		//leafs_array[leaf].max = CRAB::Vector4Df{ r*r,r,r,r };

		leaf++;
		delete t;
		return &leafs_array[posi];
	}
	if (t->state == TREE_SEMI_NODE) {
		OcElementNode* ocn = (OcElementNode*)t;

		int posi = node, pStart = node;
		nodes_array[posi] = OcElementNodeCache();
		//nodes_array[posi].min = center - CRAB::Vector4Df{ 1,1,1,0 }*r;
		//nodes_array[posi].max = center + CRAB::Vector4Df{ 1,1,1,0 }*r;

		nodes_array[posi].min = ocn->min;
		nodes_array[posi].max = ocn->max;

		node++;
		int iStart = 8;
		TreeElement* actual_tree = &nodes_array[posi];
		r *= 0.5f;
		for (int i = 0; i < 8; i++) {
			if (ocn->sons[i]->state != TREE_EMPTY_NODE) {
				nodes_array[posi].son = MakeCacheFriendlyTree(ocn->sons[i], node, leaf,
					nodes_array, leafs_array, center + cube_points[i] * r, r);
				actual_tree = nodes_array[posi].son;
				iStart = i;
				break;
			}
		}
		for (int i = iStart + 1; i < 8; i++) {
			if (ocn->sons[i]->state != TREE_EMPTY_NODE) {
				actual_tree->next = MakeCacheFriendlyTree(ocn->sons[i], node, leaf, nodes_array, leafs_array,
					center + cube_points[i] * r, r);
				actual_tree = actual_tree->next;
			}
		}
		delete ocn;
		return &nodes_array[pStart];
	}
}

void Next_Fix(TreeElement* t, TreeElement* next) {
	//if (!t)return;

	if (t->state == TREE_SEMI_NODE)
	{
		OcElementNodeCache* ocn = (OcElementNodeCache*)t;

		if (ocn->next) {
			Next_Fix(ocn->son, ocn->next);
		}
		else {
			Next_Fix(ocn->son, next);
		}
	}

	if (t->next) {
		Next_Fix(t->next, next);
	}
	else {
		t->next = next;
	}

}


OcTree::OcTree(const FaceList& faceList) {

	min = CRAB::Vector4Df{ INFINITY,INFINITY,INFINITY ,INFINITY };
	max = CRAB::Vector4Df{ -INFINITY,-INFINITY,-INFINITY,-INFINITY };


	std::cout << "Carregando faces\n";
	for (int i = 0, s = faceList.size(); i < s; i += 2) {
		const Face& f = faceList[i];
		min = CRAB::min(min, f.v1);
		max = CRAB::max(max, f.v1);

		min = CRAB::min(min, f.v2);
		max = CRAB::max(max, f.v2);

		min = CRAB::min(min, f.v3);
		max = CRAB::max(max, f.v3);
	}

	Vector4Df ax = (max - min);

	r = fmaxf(ax.x, fmaxf(ax.y, ax.z));
	center = min + (Vector4Df{ 0.5f,0.5f,0.5f,0 }*r);
	r = (center - min).length();

	std::cout << "Fazendo a arvore\n";
	tree = PovoateTree(faceList, min, max, 0);

	int nodes = 0, leafs = 0, max_size = 0, depth = 0;
	depth = Count_Nodes_Leafs(tree, nodes, leafs, max_size, depth);

	std::cout << "Otimizando a arvore\n";
	std::cout << "num nodes: " << nodes << std::endl;
	std::cout << "num leafs: " << leafs << std::endl;
	std::cout << "max size leafs: " << max_size << std::endl;
	std::cout << "depth: " << depth << std::endl;
	OcElementNodeCache* nodes_array = new OcElementNodeCache[nodes]();
	OcElementLeaf* leafs_array = new OcElementLeaf[leafs]();
	int node = 0, leaf = 0;
	tree = MakeCacheFriendlyTree(tree, node, leaf, nodes_array, leafs_array, center, r);
	Next_Fix(tree, nullptr);
}


__forceinline float horizontal_max_Vec4(__m128 x) {
	const __m128 max1 = _mm_shuffle_ps(x, x, _MM_SHUFFLE(0, 0, 0, 2));
	const __m128 max2 = _mm_max_ps(x, max1);
	const __m128 max3 = _mm_shuffle_ps(max2, max2, _MM_SHUFFLE(0, 0, 0, 1));
	const __m128 max4 = _mm_max_ps(max2, max3);
	float result = _mm_cvtss_f32(max4);
	return result;
}
__forceinline float horizontal_min_Vec4(__m128 x) {
	const __m128 max1 = _mm_shuffle_ps(x, x, _MM_SHUFFLE(0, 0, 0, 2));
	const __m128 max2 = _mm_min_ps(x, max1);
	const __m128 max3 = _mm_shuffle_ps(max2, max2, _MM_SHUFFLE(0, 0, 0, 1));
	const __m128 max4 = _mm_min_ps(max2, max3);
	float result = _mm_cvtss_f32(max4);
	return result;
}

__forceinline CRAB::Vector4Df max_Vec4(__m128 x) {
	const __m128 max1 = _mm_shuffle_ps(x, x, _MM_SHUFFLE(2, 2, 2, 2));
	const __m128 max2 = _mm_shuffle_ps(x, x, _MM_SHUFFLE(1, 1, 1, 1));
	const __m128 max21 = _mm_max_ps(max2, max1);
	const __m128 max3 = _mm_shuffle_ps(x, x, _MM_SHUFFLE(0, 0, 0, 0));
	return *(Vector4Df*)&_mm_max_ps(max21, max3);
}



Collision __fastcall TreeCollision(TreeElement* tree, const CRAB::Ray& ray) {
	Collision col{ INFINITY, Vector4Df{0,0,0,0}, nullptr };
	const CRAB::Vector4Df inv_dir = CRAB::Vector4Df{ 1,1,1,1 } / (ray.direction);

	const TreeElement* current = tree;
	while (current) {
		//const CRAB::Vector4Df c = current->min - ray.origin;
		//const float B = dot_simd(c, ray.direction);
		//const float C = dot_simd(c, c);
		//if (B * B < C - current->max.x) {
		//	current = current->next;
		//	continue;
		//}

		//const CRAB::Vector4Df c = current->min - ray.origin;
		//if (cross_simd(c, ray.direction).lengthsq() > current->max.x) {
		//	current = current->next;
		//	continue;
		//}

		const CRAB::Vector4Df v11 = (current->min - ray.origin) * inv_dir;
		const CRAB::Vector4Df v22 = (current->max - ray.origin) * inv_dir;

		const CRAB::Vector4Df minv = CRAB::min(v11, v22);
		const CRAB::Vector4Df maxv = CRAB::max(v11, v22);
		//const float min = horizontal_max_Vec4(minv);
		//const float max = horizontal_min_Vec4(maxv);
		//
		//if ((min > max) || (0 > max) || (min > col.distance)) {

		//const CRAB::Vector4Df lw{ col.distance,max,max,0 };
		//const CRAB::Vector4Df gt{ min,0,min,0 };
		//if (_mm_movemask_ps(_mm_cmplt_ps(lw,gt))){

		const CRAB::Vector4Df min = max_Vec4(minv);

		if ((_mm_movemask_ps(_mm_cmplt_ps(maxv, min))) ||
			_mm_movemask_ps(_mm_cmplt_ps(_mm_set_ps1(col.distance), min)) ||
			(_mm_movemask_ps(_mm_cmplt_ps(maxv, _mm_setzero_ps())))
			) {
			current = current->next;
			continue;
		}

		if (current->state == TREE_SEMI_NODE)
		{
			current = ((OcElementNodeCache*)current)->son;
		}
		else {
			OcElementLeaf* ocl = (OcElementLeaf*)current;

			for (Triangle& tri : ocl->faces) {
				const Vector4Df v1_r = tri.v1 - ray.origin;
				const Vector4Df r_v1 = ray.origin - tri.v1;

				const Vector4Df t = dot_simd_Vec(v1_r, tri.normal) / dot_simd_Vec(ray.direction, tri.normal);
				//if (_mm_movemask_ps(_mm_cmplt_ps(t, _mm_set_ps1(0.1f))))continue;
				//if (_mm_cvtss_f32(t) < 0.001f || _mm_cvtss_f32(t) >= col.distance)continue;
				if (_mm_cvtss_f32(t) < 0.001f)continue;
				const Vector4Df p_plane = *(Vector4Df*)&_mm_fmadd_ps(ray.direction, t, r_v1);
				//const Vector4Df p_plane = ray.direction * t + r_v1;

				const float proj1 = dot_simd(p_plane, tri.n_e1);
				const float proj2 = dot_simd(p_plane, tri.n_e2);

				const Vector4Df h{
					//1,
					col.distance,
					proj1,
					proj2,
					//tri.normal.w
					//(float)_mm_extract_ps(tri.normal,3)
					_mm_cvtss_f32(_mm_shuffle_ps(tri.normal, tri.normal, _MM_SHUFFLE(3, 3, 3, 3)))
				};
				const Vector4Df l{
					//0,
					_mm_cvtss_f32(t),
					0.000001f,
					0.000001f,
					proj2 + proj1
				};

				const __m128 gt = _mm_cmpge_ps(l, h);
				//if (_mm_testz_ps(gt, gt)) {
				if (!_mm_movemask_ps(gt)) {
					col = { _mm_cvtss_f32(t), p_plane + tri.v1, &tri };
				}
			}

			current = current->next;
		}

	}

	return col;
}

Collision __fastcall TreeCollision2(TreeElement* t, const CRAB::Vector4Df& center, float r, const CRAB::Ray& ray) {

	Collision col{ INFINITY, Vector4Df{0,0,0,0}, nullptr };

	if (t->state == TREE_EMPTY_NODE)return col;

	//const Vector4Df n = cross_simd(center, ray.direction);
	//if (n.lengthsq() > r * r) {
	//	return col;
	//}
	const float B = dot_simd(center, ray.direction);
	const float C = dot_simd(center, center);
	if (B * B < C - (r * r)) {
		return col;
	}

	if (t->state == TREE_SEMI_NODE)
	{
		OcElementNode* ocn = (OcElementNode*)t;
		r *= 0.5f;
		const Vector4Df rv{ r,r,r,r };
		for (int i = 0; i < 8; i++) {
			if (ocn->sons[i] && ocn->sons[i]->state == TREE_SEMI_NODE) {
				const Vector4Df ci = *(Vector4Df*)&_mm_fmadd_ps(cube_points[i], rv, center);
				Collision col2 = TreeCollision2(ocn->sons[i], ci, r, ray);
				if (col2.distance < col.distance)col = col2;
			}
		}
		for (int i = 0; i < 8; i++) {
			if (ocn->sons[i] && ocn->sons[i]->state == TREE_FULL_NODE) {
				const Vector4Df ci = *(Vector4Df*)&_mm_fmadd_ps(cube_points[i], rv, center);
				Collision col2 = TreeCollision2(ocn->sons[i], ci, r, ray);
				if (col2.distance < col.distance)col = col2;
			}
		}
		return col;
	}

	OcElementLeaf* ocl = (OcElementLeaf*)t;
	//for (int i = 0, s = ocl.faces.size(); i < s; i++) {
	//	const Triangle& tri = ocl.faces[i];
	for (Triangle& tri : ocl->faces) {
		const Vector4Df r_v1 = ray.origin - tri.v1;

		const Vector4Df t = dot_simd_Vec(tri.v1 - ray.origin, tri.normal) / dot_simd_Vec(ray.direction, tri.normal);

		const Vector4Df p_plane = *(Vector4Df*)&_mm_fmadd_ps(ray.direction, t, r_v1);
		//const Vector4Df p_plane = ray.direction * t + r_v1;

		const float proj1 = dot_simd(p_plane, tri.n_e1);
		const float proj2 = dot_simd(p_plane, tri.n_e2);

		const Vector4Df h{
			proj1,
			proj2,
			_mm_cvtss_f32(_mm_shuffle_ps(tri.normal, tri.normal, 3)),
			col.distance
		};
		const Vector4Df l{
			0.000001f,
			0.000001f,
			proj2 + proj1,
			_mm_cvtss_f32(t)
		};

		const Vector4Df gt = *(Vector4Df*)&_mm_cmpge_ps(l, h);
		if (_mm_movemask_ps(gt)) {
			col = { _mm_cvtss_f32(t), p_plane, &tri };
		}
		//if (proj1 >= 0.000001f && proj2 >= 0.000001f && tri.normal.w > proj2 + proj1 && col.distance > _mm_cvtss_f32(t)) {
		//	col = { _mm_cvtss_f32(t), p_plane, &tri };
		//}

	}
	return col;
}


CRAB::Collision OcTree::CollideClosest(const CRAB::Ray& ray) {

	return TreeCollision(tree, ray);
}
