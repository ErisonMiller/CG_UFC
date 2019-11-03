#pragma once
#include "DataStructures/Object.h"
#include "Geometry/Geometry.h"
#include "ImGUI.h"


#include "Cylinder.h"
#include "Quad.h"
#include "Cube.h"
#include "Cone.h"
#include "Sphere.h"
#include "Triangle.h"

static Object *selected_obj = nullptr;
static bool object_window_open = true;
static char object_name[30] = " ";


static Geometry *selected_geometry = nullptr;
static bool geometry_window_open = true;

inline void RenderGeometry() {

	if (typeid(*selected_geometry).name() == typeid(Cone).name()) {
		Cone *geometry = (Cone*)selected_geometry;
		CRAB::Vector4Df tranlate_vector = CRAB::Vector4Df{0,0,0,0};
		CRAB::Vector4Df angle_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Matrix4 m;
		ImGui::DragFloat3("Translate", (float*)&tranlate_vector, 0.5f);
		m = CRAB::translate(tranlate_vector);
		ImGui::DragFloat3("Angle", (float*)&angle_vector, 0.5f);
		m = CRAB::rotateZ(angle_vector.z)*CRAB::rotateY(angle_vector.y)*CRAB::rotateX(angle_vector.x)*m;
		
		ImGui::DragFloat3("Base", (float*)&geometry->base_center, 0.5f);
		ImGui::DragFloat3("Direction", (float*)&geometry->direction, 0.5f);
		geometry->direction.normalize();

		ImGui::DragFloat("Height", &geometry->height, 0.5f);
		ImGui::DragFloat("Radius", &geometry->radius, 0.5f);
		geometry->transform(m);
		geometry->direction.normalize();
	}

	if (typeid(*selected_geometry).name() == typeid(Cylinder).name()) {
		Cylinder *geometry = (Cylinder*)selected_geometry;
		CRAB::Vector4Df tranlate_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Matrix4 m;
		ImGui::DragFloat3("Translate", (float*)&tranlate_vector, 0.5f);
		m = CRAB::translate(tranlate_vector);
		ImGui::DragFloat3("Angle", (float*)&angle_vector, 0.5f);
		m = CRAB::rotateZ(angle_vector.z)*CRAB::rotateY(angle_vector.y)*CRAB::rotateX(angle_vector.x)*m;


		//ImGui::DragFloat3("Base", (float*)&geometry->base_center, 0.5f);
		//ImGui::DragFloat3("Direction", (float*)&geometry->direction, 0.5f);
		//geometry->direction.normalize();
		ImGui::DragFloat("Height", &geometry->height, 0.5f);
		ImGui::DragFloat("Radius", &geometry->radius, 0.5f);
		geometry->transform(m);
	}

	if (typeid(*selected_geometry).name() == typeid(Sphere).name()) {
		Sphere *geometry = (Sphere*)selected_geometry;
		ImGui::DragFloat3("Base", (float*)&geometry->center, 0.5f);
		ImGui::DragFloat("Radius", &geometry->radius, 0.5f);
	}

	if (typeid(*selected_geometry).name() == typeid(Cube).name()) {
		Cube *geometry = (Cube*)selected_geometry;
		CRAB::Vector4Df tranlate_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Matrix4 m;
		ImGui::DragFloat3("Translate", (float*)&tranlate_vector, 0.5f);
		m = CRAB::translate(tranlate_vector);
		//ImGui::DragFloat3("Angle", (float*)&angle_vector, 0.5f);
		//m = CRAB::rotateZ(angle_vector.z)*CRAB::rotateY(angle_vector.y)*CRAB::rotateX(angle_vector.x)*m;
		geometry->transform(m);
		//geometry->direction.normalize();
		// ImGui::DragFloat3("Base", (float*)&geometry->center, 0.5f);
		//ImGui::DragFloat("Radius", &geometry->r2, 0.5f);
	}

	if (typeid(*selected_geometry).name() == typeid(Quad).name()) {
		Quad *geometry = (Quad*)selected_geometry;
		CRAB::Vector4Df tranlate_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Matrix4 m;
		ImGui::DragFloat3("Translate", (float*)&tranlate_vector, 0.5f);
		m = CRAB::translate(tranlate_vector);
		//ImGui::DragFloat3("Angle", (float*)&angle_vector, 0.5f);
		//m = CRAB::rotateZ(angle_vector.z)*CRAB::rotateY(angle_vector.y)*CRAB::rotateX(angle_vector.x)*m;
		geometry->transform(m);
		//geometry->direction.normalize();
		// ImGui::DragFloat3("Base", (float*)&geometry->center, 0.5f);
		//ImGui::DragFloat("Radius", &geometry->r2, 0.5f);
	}

	if (typeid(*selected_geometry).name() == typeid(Triangle).name()) {
		Triangle *geometry = (Triangle*)selected_geometry;

		CRAB::Vector4Df tranlate_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_vector = CRAB::Vector4Df{ 0,0,0,0 };
		//CRAB::Vector4Df quartenio{ 0,1,0,0 };
		CRAB::Matrix4 m;

		ImGui::DragFloat3("Translate", (float*)&tranlate_vector, 0.5f);
		m = CRAB::translate(tranlate_vector);
		ImGui::DragFloat3("Angle", (float*)&angle_vector, 0.5f);
		m = CRAB::rotateZ(angle_vector.z)*CRAB::rotateY(angle_vector.y)*CRAB::rotateX(angle_vector.x)*m;

		//ImGui::DragFloat4("Quart", (float*)&quartenio, 0.5f);
		//m = CRAB::rotateArbitrary(quartenio.w, quartenio)*m;
		
		geometry->transform(m);
	}



}

inline void RenderObject() {
	if (object_window_open && selected_obj) {
		ImGui::Begin("Object Properties", &object_window_open, ImGuiWindowFlags_AlwaysAutoResize);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Object Name:");// ImGui::SameLine();
		strcpy_s(object_name, selected_obj->getName().c_str());
		if (ImGui::InputText("", object_name, 30, ImGuiInputTextFlags_EnterReturnsTrue)) {
			selected_obj->name = object_name;
		}
		ImGui::DragFloat3("Posi", (float*)&selected_obj->position, 0.5f);

		if(ImGui::CollapsingHeader("Material Properties:")){
			ImGui::ColorEdit4("Color Ambient", (float*)&selected_obj->getMaterial()->ka, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Color Difuse", (float*)&selected_obj->getMaterial()->kd, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Color Specular", (float*)&selected_obj->getMaterial()->ks, ImGuiColorEditFlags_NoInputs);
			ImGui::DragFloat("Alpha", &selected_obj->getMaterial()->shininess);
		}

		selected_geometry = selected_obj->getGeometry();

		if (ImGui::CollapsingHeader("Geometry Properties:")) {
			ImGui::Text(typeid(*selected_geometry).name());
			RenderGeometry();
		}

		ImGui::End();
	}
}
