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
		ImGui::InputFloat4("Base", (float*)&geometry->base_center);
		ImGui::InputFloat4("Direction", (float*)&geometry->direction);
		geometry->direction.normalize();
		ImGui::InputFloat("Height", &geometry->height);
		ImGui::InputFloat("Radius", &geometry->radius);
		geometry->top_vertex = geometry->base_center + geometry->direction*geometry->height;
	}

	if (typeid(*selected_geometry).name() == typeid(Cylinder).name()) {
		Cylinder *geometry = (Cylinder*)selected_geometry;
		ImGui::InputFloat4("Base", (float*)&geometry->base_center);
		ImGui::InputFloat4("Direction", (float*)&geometry->direction);
		geometry->direction.normalize();
		ImGui::InputFloat("Height", &geometry->height);
		ImGui::InputFloat("Radius", &geometry->radius);
	}

	if (typeid(*selected_geometry).name() == typeid(Sphere).name()) {
		Sphere *geometry = (Sphere*)selected_geometry;
		ImGui::InputFloat4("Base", (float*)&geometry->center);
		ImGui::InputFloat("Radius", &geometry->radius);
	}

}

inline void RenderObject() {
	if (object_window_open && selected_obj) {
		ImGui::Begin("Object Properties", &object_window_open, ImGuiWindowFlags_AlwaysAutoResize);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Object Name:"); ImGui::SameLine();
		strcpy_s(object_name, selected_obj->getName().c_str());
		if (ImGui::InputText("", object_name, 30, ImGuiInputTextFlags_EnterReturnsTrue)) {
			selected_obj->name = object_name;
		}
		ImGui::InputFloat3("Position", (float*)&selected_obj->position);

		if(ImGui::CollapsingHeader("Material Properties:")){
			ImGui::ColorEdit4("Color Ambient", (float*)&selected_obj->getMaterial()->ka, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Color Difuse", (float*)&selected_obj->getMaterial()->kd, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Color Specular", (float*)&selected_obj->getMaterial()->ks, ImGuiColorEditFlags_NoInputs);
			ImGui::InputFloat("Alpha", &selected_obj->getMaterial()->alfa);
		}

		selected_geometry = selected_obj->getGeometry();

		if (ImGui::CollapsingHeader("Geometry Properties:")) {
			ImGui::Text(typeid(*selected_geometry).name());
			RenderGeometry();
		}

		ImGui::End();
	}
}
