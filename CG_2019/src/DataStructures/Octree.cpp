#include "Octree.h"

#include <iostream>

using namespace CRAB;


#define MAX_TREE_SIZE 32
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

bool Line_Sphere(const Vector4Df &o, const Vector4Df &d, const Vector4Df &c, float r) {
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

TreeElement* PovoateTree(const FaceList& fatherFaceList, const CRAB::Vector4Df &center, float r, int depth) {
	if (depth > 15) {
		return new OcElementLeaf(fatherFaceList);
	}

	FaceList faceList;
	if (depth) {

		for (int i = 0, s = fatherFaceList.faces.size(); i < s; i+=2) {
			const Face& f = fatherFaceList.faces[i];
		//for (const Face& f : fatherFaceList) {
			if ((f.v1 - center).lengthsq() <= r * r ||
				(f.v2 - center).lengthsq() <= r * r ||
				(f.v3 - center).lengthsq() <= r * r) {
				faceList.faces.push_back(f);
				faceList.faces.push_back(fatherFaceList.faces[i+1]);
				continue;
			}

			//Triangle tri(f.v1, f.v2, f.v3);
			//
			//CRAB::Collision col = tri.CollideClosest(CRAB::Ray{ center, tri.normal });
			//
			//if (col.distance <= r + 0.001f && col.distance >= -(r+ 0.001f)) {
			//	faceList.push_back(f);
			//	continue;
			//}

			const CRAB::Vector4Df v0v1 = f.v2 - f.v1;
			const CRAB::Vector4Df v0v2 = f.v3 - f.v1;

			if (Line_Sphere(f.v1, v0v1, center, r) || Line_Sphere(f.v1, v0v2, center, r) || Line_Sphere(f.v2, f.v3 - f.v2, center, r)) {
				//faceList.push_back(f);

				faceList.faces.push_back(f);
				faceList.faces.push_back(fatherFaceList.faces[i + 1]);
				continue;
			}

			const CRAB::Vector4Df dir = cross_simd(v0v1, v0v2).to_unitary();
			const CRAB::Vector4Df pvec = cross_simd(dir, v0v2);
			float det = dot_simd(v0v1, pvec);

			float invDet = 1.0f / det;

			CRAB::Vector4Df tvec = center - f.v1;
			float u = dot_simd(tvec, pvec) * invDet;
			if (u < 0 || u > 1) continue;

			CRAB::Vector4Df qvec = cross_simd(tvec, v0v1);
			float v = dot_simd(dir, qvec) * invDet;
			if (v < 0 || u + v > 1) continue;

			float t = CRAB::dot(v0v2, qvec) * invDet;
			if (t < -r || t > r) continue;

			//faceList.push_back(f);

			faceList.faces.push_back(f);
			faceList.faces.push_back(fatherFaceList.faces[i + 1]);
		}
	}
	else {
		faceList = fatherFaceList;
	}

	if(faceList.faces.empty()) return new TreeElement(TREE_EMPTY_NODE);
	
	const int cut_size = 0;
	
	if (faceList.faces.size() > MAX_TREE_SIZE && (faceList.faces.size() <= fatherFaceList.faces.size() - cut_size * MAX_TREE_SIZE || depth == 0))
	{
		r *= 0.5f;
		OcElementNode* ocn = new OcElementNode();
		if (depth) {
			for (int i = 0; i < 8; i++) {
				ocn->sons[i] = PovoateTree(faceList, center + cube_points[i] * r, r, depth + 1);
			}
		}
		else {
			#pragma omp parallel for schedule(dynamic)
			for (int i = 0; i < 8; i++) {
				ocn->sons[i] = PovoateTree(faceList, center + cube_points[i] * r, r, depth + 1);
			}
		}
		int equals = 0;
		int emp = 0;
		for (int i = 0; i < 8; i++) {
			if (ocn->sons[i]->state == TREE_FULL_NODE) {
				OcElementLeaf* ocl = (OcElementLeaf*)ocn->sons[i];
				if (ocl->faces.size() == faceList.faces.size()) {
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
		return ocn;
	}
	if (faceList.faces.size() > fatherFaceList.faces.size() - cut_size * MAX_TREE_SIZE) {
		return new OcElementLeaf(fatherFaceList);
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


void Count_Nodes_Leafs(TreeElement* t, int& nodes, int& leafs) {
	if (t->state == TREE_FULL_NODE) {
		leafs++;
	}
	if (t->state == TREE_SEMI_NODE) {
		OcElementNode* ocn = (OcElementNode*)t;
		for (int i = 0; i < 8; i++) {
			Count_Nodes_Leafs(ocn->sons[i], nodes, leafs);
		}
		nodes++;
	}

}

TreeElement* MakeCacheFriendlyTree(TreeElement* t, int& node, int& leaf, OcElementNodeCache* nodes_array,
	OcElementLeaf* leafs_array, const CRAB::Vector4Df &center, float r) {

	if (t->state == TREE_FULL_NODE) {
		//return t;
		int posi = leaf;
		leafs_array[leaf] = *(OcElementLeaf*)t;
		//leafs_array[leaf].center = center;
		//leafs_array[leaf].r2 = (r * r);
		leafs_array[leaf].min = center - CRAB::Vector4Df{ 1,1,1,0 }*r;
		leafs_array[leaf].max = center + CRAB::Vector4Df{ 1,1,1,0 }*r;

		leaf++;
		delete t;
		return &leafs_array[posi];
	}
	if (t->state == TREE_SEMI_NODE) {
		OcElementNode* ocn = (OcElementNode*)t;
		
		int posi = node, pStart = node;
		nodes_array[posi] = OcElementNodeCache();
		//nodes_array[posi].r2 = (r * r);
		//nodes_array[posi].center = center;
		nodes_array[posi].min = center - CRAB::Vector4Df{ 1,1,1,0 }*r;
		nodes_array[posi].max = center + CRAB::Vector4Df{ 1,1,1,0 }*r;
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
	}else{
		t->next = next;
	}

}


OcTree::OcTree(const FaceList &faceList) {

	CRAB::Vector4Df min{ INFINITY,INFINITY,INFINITY ,INFINITY };
	CRAB::Vector4Df max{ -INFINITY,-INFINITY,-INFINITY,-INFINITY };


	std::cout << "Carregando faces\n";
	for (int i = 0, s = faceList.faces.size(); i < s; i+=2) {
		const Face& f = faceList.faces[i];
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
	tree = PovoateTree(faceList, center, r, 0);

	int nodes = 0, leafs = 0;
	Count_Nodes_Leafs(tree, nodes, leafs);

	std::cout << "Otimizando a arvore\n";
	std::cout << "num nodes: " << nodes << std::endl;
	std::cout << "num leafs: " << leafs << std::endl;
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
		//const CRAB::Vector4Df c = current->center - ray.origin;
		//const float B = dot_simd(c, ray.direction);
		//const float C = dot_simd(c, c) ;
		//if (B * B < C - current->r2) {
		//	current = current->next;
		//	continue;
		//}
		const CRAB::Vector4Df v11 = (current->min - ray.origin) * inv_dir;
		const CRAB::Vector4Df v22 = (current->max - ray.origin) * inv_dir;
		
		//const CRAB::Vector4Df v11 = *(Vector4Df*)&_mm_fmsub_ps(current->min, inv_dir, inv_or);
		//const CRAB::Vector4Df v22 = *(Vector4Df*)&_mm_fmsub_ps(current->max, inv_dir, inv_or);

		const CRAB::Vector4Df minv = CRAB::min(v11, v22);
		const CRAB::Vector4Df maxv = CRAB::max(v11, v22);
		//const float min = horizontal_max_Vec4(minv);
		//const float max = horizontal_min_Vec4(maxv);
		//
		//if ((min > max) || (max < 0) || (min > col.distance)) {

		//const CRAB::Vector4Df lw{ col.distance,max,max,0 };
		//const CRAB::Vector4Df gt{ min,0,min,0 };
		//if (_mm_movemask_ps(_mm_cmplt_ps(lw,gt))){
		
		const CRAB::Vector4Df min = max_Vec4(minv);
		if ((_mm_movemask_ps(_mm_cmplt_ps(maxv, min))) ||
			(_mm_movemask_ps(_mm_cmplt_ps(maxv, _mm_setzero_ps()))) ||
			_mm_movemask_ps(_mm_cmplt_ps(_mm_set_ps1(col.distance), min))){
			current = current->next;
			continue;
		}
		
		if (current->state == TREE_SEMI_NODE)
		{
			current = ((OcElementNodeCache*)current)->son;
		}else{
			OcElementLeaf* ocl = (OcElementLeaf*)current;
			
			for (Triangle& tri : ocl->faces) {
				const Vector4Df v1_r = tri.v1 - ray.origin;
				const Vector4Df r_v1 = ray.origin - tri.v1;

				const Vector4Df t = dot_simd_Vec(v1_r, tri.normal) / dot_simd_Vec(ray.direction, tri.normal);
				//if (_mm_movemask_ps(_mm_cmplt_ps(t, _mm_set_ps1(0.1f))))continue;
				//if (_mm_cvtss_f32(t) < 0.001f || _mm_cvtss_f32(t) >= col.distance)continue;
				if (_mm_cvtss_f32(t) < 0.001f)continue;
				//const Vector4Df p_plane = *(Vector4Df*)&_mm_fmadd_ps(ray.direction, t, r_v1);
				const Vector4Df p_plane = ray.direction * t + r_v1;

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
			if(ocn->sons[i] && ocn->sons[i]->state == TREE_SEMI_NODE){
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

	OcElementLeaf *ocl = (OcElementLeaf*)t;
	//for (int i = 0, s = ocl.faces.size(); i < s; i++) {
	//	const Triangle& tri = ocl.faces[i];
	for(Triangle & tri : ocl->faces){
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
