//oct continuos

#include "Octree.h"

#include <iostream>

using namespace CRAB;


#define MAX_TREE_SIZE 16
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



void Project(const CRAB::Vector4Df *vertices, int size, CRAB::Vector4Df normal,
	float &out_min, float &out_max)
{
	out_min = INFINITY;
	out_max = -INFINITY;
	for(int i=0; i<size; i++)
	{
		double val = dot_simd(normal, vertices[i]);
		if (val < out_min) out_min = val;
		if (val > out_max) out_max = val;
	}
}

bool IsIntersecting(const Face &triangle, const CRAB::Vector4Df& bmin, const CRAB::Vector4Df& bmax)
{
	float triangleMin, triangleMax;
	float boxMin, boxMax;

	// Test the box normals (x-, y- and z-axes)
	CRAB::Vector4Df box_vertices[8]{
		bmin,
		CRAB::Vector4Df{bmin.x, bmax.y, bmin.z},
		CRAB::Vector4Df{bmax.x, bmin.y, bmin.z},
		CRAB::Vector4Df{bmax.x, bmax.y, bmin.z},
		bmax,
		CRAB::Vector4Df{bmin.x, bmin.y, bmax.z},
		CRAB::Vector4Df{bmax.x, bmin.y, bmax.z},
		CRAB::Vector4Df{bmin.x, bmax.y, bmax.z},
		//	triangle.v1, triangle.v2;
	};



	CRAB::Vector4Df boxNormals[3]{
		CRAB::Vector4Df{1,0,0,0},
		CRAB::Vector4Df{0,1,0,0},
		CRAB::Vector4Df{0,0,1,0}
	};

	for (int i = 0; i < 3; i++)
	{
		Project(triangle.vertices, 3, boxNormals[i], triangleMin, triangleMax);
		if (triangleMax < bmin._v[i] || triangleMin > bmax._v[i])
			return false; // No intersection possible.
	}

	// Test the triangle normal
	CRAB::Vector4Df normal = cross_simd(triangle.v2 - triangle.v1, triangle.v3 - triangle.v1);
	float triangleOffset = dot_simd(normal, triangle.v1);
	Project(box_vertices, 8, normal, boxMin, boxMax);
	if (boxMax < triangleOffset || boxMin > triangleOffset)
		return false; // No intersection possible.

	// Test the nine edge cross-products
	CRAB::Vector4Df triangleEdges[3]{
		triangle.v1 - triangle.v2,
		triangle.v2 - triangle.v3,
		triangle.v3 - triangle.v1
	};

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			// The box normals are the same as it's edge tangents
			CRAB::Vector4Df axis = cross_simd(triangleEdges[i],boxNormals[j]);
			if (axis.x == 0 && axis.y == 0 && axis.z == 0)return false;
			Project(box_vertices, 8, axis, boxMin, boxMax);
			Project(triangle.vertices, 3, axis, triangleMin, triangleMax);
			if (boxMax <= triangleMin || boxMin >= triangleMax)
				return false; // No intersection possible
		}

	// No separating axis found.
	return true;
}



TreeElement* PovoateTree(FaceList& fatherFaceList, CRAB::Vector4Df min, CRAB::Vector4Df max, int depth) {
	
	if (fatherFaceList.faces.empty()) {

		fatherFaceList.faces.clear();
		fatherFaceList.faces.shrink_to_fit();

		TreeElement *t = new TreeElement(TREE_EMPTY_NODE);
		t->min = Vector4Df{ INFINITY,INFINITY,INFINITY,INFINITY };
		t->max = Vector4Df{ -INFINITY,-INFINITY,-INFINITY,-INFINITY };
		return t;
	}
	CRAB::Vector4Df _min0 = min, _max0 = max;
	if (depth) {
		_min0 = Vector4Df{ INFINITY,INFINITY,INFINITY,INFINITY };
		_max0 = Vector4Df{ -INFINITY,-INFINITY,-INFINITY,-INFINITY };
	
		for (int i = 0, s = fatherFaceList.faces.size(); i < s; i += 2) {
			const Face& f = fatherFaceList.faces[i];
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

	if (depth >= 15 || fatherFaceList.faces.size() <= MAX_TREE_SIZE*3) {
		TreeElement* t = new OcElementLeaf(fatherFaceList);
		t->min = _min0;
		t->max = _max0;
		
		fatherFaceList.faces.clear();
		fatherFaceList.faces.shrink_to_fit();

		return t;
	}

	CRAB::Vector4Df facesCenter = ((_max0 + _min0) * 0.5f);


	CRAB::Vector4Df min_vec[8]{
		CRAB::Vector4Df{_min0.x,		_min0.y,		_min0.z, 1},
		CRAB::Vector4Df{facesCenter.x,	_min0.y,		_min0.z, 1},
		CRAB::Vector4Df{facesCenter.x,	facesCenter.y,	_min0.z, 1},
		CRAB::Vector4Df{_min0.x,		facesCenter.y,	_min0.z, 1},


		CRAB::Vector4Df{_min0.x,		_min0.y,		facesCenter.z, 1},
		CRAB::Vector4Df{facesCenter.x,	_min0.y,		facesCenter.z, 1},
		CRAB::Vector4Df{facesCenter.x,	facesCenter.y,	facesCenter.z, 1},
		CRAB::Vector4Df{_min0.x,		facesCenter.y,	facesCenter.z, 1}

	};

	CRAB::Vector4Df max_vec[8]{
		CRAB::Vector4Df{facesCenter.x,	facesCenter.y,	facesCenter.z, 1},
		CRAB::Vector4Df{_max0.x,		facesCenter.y,	facesCenter.z, 1},
		CRAB::Vector4Df{_max0.x,		_max0.y,		facesCenter.z, 1},
		CRAB::Vector4Df{facesCenter.x,	_max0.y,		facesCenter.z, 1},


		CRAB::Vector4Df{facesCenter.x,	facesCenter.y,	_max0.z, 1},
		CRAB::Vector4Df{_max0.x,		facesCenter.y,	_max0.z, 1},
		CRAB::Vector4Df{_max0.x,		_max0.y,		_max0.z, 1},
		CRAB::Vector4Df{facesCenter.x,	_max0.y,		_max0.z, 1}

	};


	FaceList faceList[8];
	
	//CRAB::Vector4Df facesCenter{0,0,0,0};
	//for (int i = 0, s = fatherFaceList.size(); i < s; i += 2) {
	//	facesCenter = facesCenter + fatherFaceList[i].getCenter();
	//}
	//facesCenter = facesCenter / (fatherFaceList.size() * 0.5f);
	
	for (int i = 0, s = fatherFaceList.faces.size(); i < s; i += 2) {
		const Face& f = fatherFaceList.faces[i];
		
		for (int j = 0; j < 8; j++) {
			if (IsIntersecting(f, min_vec[j], max_vec[j])) {
				faceList[j].faces.push_back(f);
				faceList[j].faces.push_back(fatherFaceList.faces[i+1]);
			}
		}
	}



	int equals0 = 0;
	int zeros = 0;
	for (int i = 0; i < 8; i++) {
		
		if (faceList[i].faces.size() >= fatherFaceList.faces.size()*0.55) {
			equals0++;
		}
		if (faceList[i].faces.empty()) {
			zeros++;
		}
	}
	if (equals0 == 8) {
		TreeElement* t = new OcElementLeaf(fatherFaceList);
		t->min = _min0;
		t->max = _max0;
	
		fatherFaceList.faces.clear();
		fatherFaceList.faces.shrink_to_fit();
	
		return t;
	}
	if (zeros == 8) {
		fatherFaceList.faces.clear();
		fatherFaceList.faces.shrink_to_fit();

		TreeElement* t = new TreeElement(TREE_EMPTY_NODE);
		t->min = Vector4Df{ INFINITY,INFINITY,INFINITY,INFINITY };
		t->max = Vector4Df{ -INFINITY,-INFINITY,-INFINITY,-INFINITY };
		return t;
	}

	fatherFaceList.faces.clear();
	fatherFaceList.faces.shrink_to_fit();
	
	

	OcElementNode* ocn = new OcElementNode();
	

	if (depth) {
		for (int i = 0; i < 8; i++) {
			ocn->sons[i] = PovoateTree(faceList[i], min_vec[i], max_vec[i], depth + 1);
		}
	}
	else {
		#pragma openmp parallel for
		for (int i = 0; i < 8; i++) {
			ocn->sons[i] = PovoateTree(faceList[i], min_vec[i], max_vec[i], depth + 1);
		}

	}
	
	ocn->min = _min0;
	ocn->max = _max0;
	return ocn;
	
}


int Count_Nodes_Leafs(TreeElement* t, int& nodes, int& leafs, int& max_size, int& faces_sum, int depth) {
	if (t->state == TREE_FULL_NODE) {
		OcElementLeaf* ocn = (OcElementLeaf*)t;
		faces_sum += ocn->faces.size();
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
			d = Count_Nodes_Leafs(ocn->sons[i], nodes, leafs, max_size, faces_sum, depth + 1);
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
		((OcElementLeaf*)t)->faces.clear();
		((OcElementLeaf*)t)->faces.shrink_to_fit();
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
			else {
				delete ocn->sons[i];
			}
		}
		for (int i = iStart + 1; i < 8; i++) {
			if (ocn->sons[i]->state != TREE_EMPTY_NODE) {
				actual_tree->next = MakeCacheFriendlyTree(ocn->sons[i], node, leaf, nodes_array, leafs_array,
					center + cube_points[i] * r, r);
				actual_tree = actual_tree->next;
			}
			else {
				delete ocn->sons[i];
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


OcTree::OcTree(FaceList& faceList) {

	min = CRAB::Vector4Df{ INFINITY,INFINITY,INFINITY ,INFINITY };
	max = CRAB::Vector4Df{ -INFINITY,-INFINITY,-INFINITY,-INFINITY };


	std::cout << "Carregando faces\n";
	for (int i = 0, s = faceList.faces.size(); i < s; i += 2) {
		const Face& f = faceList.faces[i];
		min = CRAB::min(min, f.v1);
		max = CRAB::max(max, f.v1);

		min = CRAB::min(min, f.v2);
		max = CRAB::max(max, f.v2);

		min = CRAB::min(min, f.v3);
		max = CRAB::max(max, f.v3);
	}
	std::cout << faceList.faces.size()*0.5f << " faces\n";
	Vector4Df ax = (max - min);

	r = fmaxf(ax.x, fmaxf(ax.y, ax.z));
	center = min + (Vector4Df{ 0.5f,0.5f,0.5f,0 }*r);
	r = (center - min).length();

	std::cout << "Fazendo a arvore\n";
	tree = PovoateTree(faceList, min, max, 0);
	std::cout << "Feito\n";
	int nodes = 0, leafs = 0, max_size = 0, depth = 0, faces_sum = 0;
	depth = Count_Nodes_Leafs(tree, nodes, leafs, max_size, faces_sum, depth);

	std::cout << "Otimizando a arvore\n";
	std::cout << "num nodes: " << nodes << std::endl;
	std::cout << "num leafs: " << leafs << std::endl;
	std::cout << "max size leafs: " << max_size << std::endl;
	std::cout << "total faces: " << faces_sum << std::endl;
	std::cout << "media faces: " << faces_sum/((float)leafs) << std::endl;
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

		//if ((_mm_movemask_ps(_mm_cmplt_ps(maxv, min))) ||
		//	_mm_movemask_ps(_mm_cmplt_ps(_mm_set_ps1(col.distance), min)) ||
		//	(_mm_movemask_ps(_mm_cmplt_ps(maxv, _mm_setzero_ps())))
		//	) {
		if ((_mm_movemask_ps(_mm_cmplt_ps(maxv, min))) |
			col.distance < _mm_cvtss_f32(min) |
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