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

	if (typeid(*selected_geometry) == typeid(Cone)) {
		Cone *geometry = (Cone*)selected_geometry;

		CRAB::Vector4Df tranlate_vector = CRAB::Vector4Df{0,0,0,0};
		CRAB::Vector4Df angle_origin_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_object_vector = CRAB::Vector4Df{ 0,0,0,0 };
		static CRAB::Vector4Df reflection_normal_vector = CRAB::Vector4Df{ 0,0,0,0 };

		static CRAB::Vector4Df quaternion{ 0,1,0,0 };
		float angle_quat_vector = 0.0f;

		CRAB::Matrix4 m = CRAB::Identity();

		if (ImGui::DragFloat3("Translate", (float*)&tranlate_vector, 0.5f)); {
			m = CRAB::translate(tranlate_vector);
		}
		
		if (ImGui::DragFloat3("Angle (Origin)", (float*)&angle_origin_vector, 0.5f)) {
			m = CRAB::rotateZ(angle_origin_vector.z)*CRAB::rotateY(angle_origin_vector.y)*CRAB::rotateX(angle_origin_vector.x)*m;
		}

		if (ImGui::DragFloat3("Angle (Base Center)", (float*)&angle_object_vector, 0.5f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->base_center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->base_center));
			m = m_back * CRAB::rotateZ(angle_object_vector.z)*CRAB::rotateY(angle_object_vector.y)*CRAB::rotateX(angle_object_vector.x) * m_to_origin;
		}

		if (ImGui::DragFloat("Angle (Quaternion)", (float*)&angle_quat_vector, 0.5f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->base_center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->base_center));
			CRAB::Vector4Df quaternio_final = quaternion.to_unitary();

			m = m_back * CRAB::rotateArbitrary(angle_quat_vector, quaternio_final) * m_to_origin;
		}

		if (ImGui::DragFloat3("Quart", (float*)&quaternion, 0.1f)) {
			//
		}

		if (ImGui::DragFloat("Height", &geometry->height, 0.5f)) {
			//
		}
		
		if (ImGui::DragFloat("Radius", &geometry->radius, 0.5f)) {
			//
		}

		if (ImGui::DragFloat3("Normal Reflection Plan", (float*)&reflection_normal_vector, 0.5f)) {
			//
		}

		if (ImGui::Button("Reflect"))
		{
			CRAB::Matrix4 m_to_origin = translate((geometry->base_center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->base_center));
			if (reflection_normal_vector.length() > 0)
			{
				reflection_normal_vector = reflection_normal_vector.to_unitary();
			}
			m = m_back * reflectionArbitrary(reflection_normal_vector) * m_to_origin;
		}

		geometry->transform(m);
		geometry->direction.normalize();
	}

	if (typeid(*selected_geometry) == typeid(Cylinder)) {
		Cylinder *geometry = (Cylinder*)selected_geometry;

		CRAB::Vector4Df tranlate_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_origin_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_object_vector = CRAB::Vector4Df{ 0,0,0,0 };
		static CRAB::Vector4Df reflection_normal_vector = CRAB::Vector4Df{ 0,0,0,0 };

		static CRAB::Vector4Df quaternion{ 0,1,0,0 };
		float angle_quat_vector = 0.0f;

		CRAB::Matrix4 m = CRAB::Identity();
		
		if (ImGui::DragFloat3("Translate", (float*)&tranlate_vector, 0.5f)) {
			m = CRAB::translate(tranlate_vector);
		}
		
		if (ImGui::DragFloat3("Angle (Origin)", (float*)&angle_origin_vector, 0.5f)) {
			m = CRAB::rotateZ(angle_origin_vector.z)*CRAB::rotateY(angle_origin_vector.y)*CRAB::rotateX(angle_origin_vector.x)*m;
		}

		if (ImGui::DragFloat3("Angle (Base Center)", (float*)&angle_object_vector, 0.5f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->base_center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->base_center));
			m = m_back * CRAB::rotateZ(angle_object_vector.z)*CRAB::rotateY(angle_object_vector.y)*CRAB::rotateX(angle_object_vector.x) * m_to_origin;
		}

		if (ImGui::DragFloat("Quaternion Angle (W)", (float*)&angle_quat_vector, 0.5f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->base_center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->base_center));
			CRAB::Vector4Df quaternio_final = quaternion.to_unitary();

			m = m_back * CRAB::rotateArbitrary(angle_quat_vector, quaternio_final) * m_to_origin;
		}

		if (ImGui::DragFloat3("Quaternion Vector (V)", (float*)&quaternion, 0.1f)) {
			//
		}
		
		if (ImGui::DragFloat("Height", &geometry->height, 0.5f)) {
			//
		}

		if (ImGui::DragFloat("Radius", &geometry->radius, 0.5f)) {
			//
		}

		if (ImGui::DragFloat3("Normal Reflection Plan", (float*)&reflection_normal_vector, 0.5f)) {
			//
		}

		if (ImGui::Button("Reflect"))
		{
			CRAB::Matrix4 m_to_origin = translate((geometry->base_center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->base_center));
			if (reflection_normal_vector.length() > 0)
			{
				reflection_normal_vector = reflection_normal_vector.to_unitary();
			}
			m = m_back * reflectionArbitrary(reflection_normal_vector) * m_to_origin;
		}

		geometry->transform(m);
		geometry->direction.normalize();
	}

	if (typeid(*selected_geometry) == typeid(Sphere)) {
		Sphere *geometry = (Sphere*)selected_geometry;
		CRAB::Vector4Df tranlate_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_origin_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_object_vector = CRAB::Vector4Df{ 0,0,0,0 };
		static CRAB::Vector4Df reflection_normal_vector = CRAB::Vector4Df{ 0,0,0,0 };

		static CRAB::Vector4Df quaternion{ 0,1,0,0 };
		float angle_quat_vector = 0.0f;

		CRAB::Matrix4 m = CRAB::Identity();

		if (ImGui::DragFloat3("Translate", (float*)&tranlate_vector, 0.5f)) {
			m = CRAB::translate(tranlate_vector);
		}

		if (ImGui::DragFloat3("Angle (Origin)", (float*)&angle_origin_vector, 0.5f)) {
			m = CRAB::rotateZ(angle_origin_vector.z)*CRAB::rotateY(angle_origin_vector.y)*CRAB::rotateX(angle_origin_vector.x)*m;
		}

		if (ImGui::DragFloat3("Angle (Base Center)", (float*)&angle_object_vector, 0.5f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->center));
			m = m_back * CRAB::rotateZ(angle_object_vector.z)*CRAB::rotateY(angle_object_vector.y)*CRAB::rotateX(angle_object_vector.x) * m_to_origin;
		}

		if (ImGui::DragFloat("Radius", &geometry->radius, 0.5f)) {
			//
		}

		if (ImGui::DragFloat("Quaternion Angle (W)", (float*)&angle_quat_vector, 0.5f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->center));
			CRAB::Vector4Df quaternio_final = quaternion.to_unitary();

			m = m_back * CRAB::rotateArbitrary(angle_quat_vector, quaternio_final) * m_to_origin;
		}

		if (ImGui::DragFloat3("Quaternion Vector (V)", (float*)&quaternion, 0.1f)) {
			//
		}

		if (ImGui::DragFloat3("Normal Reflection Plan", (float*)&reflection_normal_vector, 0.5f)) {
			//
		}

		if (ImGui::Button("Reflect"))
		{
			CRAB::Matrix4 m_to_origin = translate((geometry->center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->center));
			if (reflection_normal_vector.length() > 0)
			{
				reflection_normal_vector = reflection_normal_vector.to_unitary();
			}
			m = m_back * reflectionArbitrary(reflection_normal_vector) * m_to_origin;
		}

		geometry->transform(m);
	}

	if (typeid(*selected_geometry) == typeid(Cube)) {
		Cube *geometry = (Cube*)selected_geometry;

		CRAB::Vector4Df tranlate_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_origin_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_object_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df scale_vector = CRAB::Vector4Df{ 1,1,1,0 };
		static CRAB::Vector4Df reflection_normal_vector = CRAB::Vector4Df{ 0,0,0,0 };

		static CRAB::Vector4Df quaternion{ 0,1,0,0 };
		float angle_quat_vector = 0.0f;

		CRAB::Matrix4 m = CRAB::Identity();

		if (ImGui::DragFloat3("Translate", (float*)&tranlate_vector, 0.5f)) {
			m = CRAB::translate(tranlate_vector)*m;
		}
		if (ImGui::DragFloat3("Angle (Origin)", (float*)&angle_origin_vector, 0.5f)) {
			m = CRAB::rotateZ(angle_origin_vector.z)*CRAB::rotateY(angle_origin_vector.y)*CRAB::rotateX(angle_origin_vector.x)*m;
		}

		if (ImGui::DragFloat3("Angle (Base Center)", (float*)&angle_object_vector, 0.5f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->center));
			m = m_back * CRAB::rotateZ(angle_object_vector.z)*CRAB::rotateY(angle_object_vector.y)*CRAB::rotateX(angle_object_vector.x) * m_to_origin;
		}

		if (ImGui::DragFloat("Quaternion Angle (W)", (float*)&angle_quat_vector, 0.5f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->center));
			CRAB::Vector4Df quaternio_final = quaternion.to_unitary();

			m = m_back * CRAB::rotateArbitrary(angle_quat_vector, quaternio_final) * m_to_origin;
		}

		if (ImGui::DragFloat3("Quaternion Vector (V)", (float*)&quaternion, 0.1f)) {
			//
		}

		if (ImGui::DragFloat3("Scale", (float*)&scale_vector, 0.1f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->center));
			m = CRAB::resizeMatrix(scale_vector.x, scale_vector.y, scale_vector.z);
		}

		if (ImGui::DragFloat3("Normal Reflection Plan", (float*)&reflection_normal_vector, 0.5f)) {
			//
		}

		if (ImGui::Button("Reflect"))
		{
			CRAB::Matrix4 m_to_origin = translate((geometry->center) * -1);
			CRAB::Matrix4 m_back = translate((geometry->center));
			if (reflection_normal_vector.length() >0)
			{
				reflection_normal_vector = reflection_normal_vector.to_unitary();
			}
			m = m_back * reflectionArbitrary(reflection_normal_vector) * m_to_origin;
			
		}


		geometry->transform(m);
	}

	if (typeid(*selected_geometry) == typeid(Quad)) {
		Quad *geometry = (Quad*)selected_geometry;
		
		CRAB::Vector4Df translate_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_origin_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_object_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df scale_vector = CRAB::Vector4Df{ 1,1,1,0 };
		static CRAB::Vector4Df reflection_normal_vector = CRAB::Vector4Df{ 0,0,0,0 };
		
		static CRAB::Vector4Df quaternion{ 0,1,0,0 };
		float angle_quat_vector = 0.0f;

		CRAB::Matrix4 m = CRAB::Identity();

		if (ImGui::DragFloat3("Translate", (float*)&translate_vector, 0.5f)) {
			m = CRAB::translate(translate_vector);
		}
		if (ImGui::DragFloat3("Angle (Origin)", (float*)&angle_origin_vector, 0.5f)) {
			m = CRAB::rotateZ(angle_origin_vector.z)*CRAB::rotateY(angle_origin_vector.y)*CRAB::rotateX(angle_origin_vector.x)*m;
		}

		if (ImGui::DragFloat3("Angle (Base Vertex)", (float*)&angle_object_vector, 0.5f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->v1) * -1);
			CRAB::Matrix4 m_back = translate((geometry->v1));
			m = m_back * CRAB::rotateZ(angle_object_vector.z)*CRAB::rotateY(angle_object_vector.y)*CRAB::rotateX(angle_object_vector.x) * m_to_origin;
		}

		if (ImGui::DragFloat("Quaternion Angle (W)", (float*)&angle_quat_vector, 0.5f)){
			CRAB::Matrix4 m_to_origin = translate((geometry->v1) * -1);
			CRAB::Matrix4 m_back = translate((geometry->v1));
			CRAB::Vector4Df quaternio_final = quaternion.to_unitary();

			m = m_back * CRAB::rotateArbitrary(angle_quat_vector, quaternio_final) * m_to_origin;
		}
		
		if (ImGui::DragFloat3("Quaternion Vector (V)", (float*)&quaternion, 0.1f)) {
			//
		}

		if (ImGui::DragFloat3("Scale", (float*)&scale_vector, 0.1f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->v1) * -1);
			CRAB::Matrix4 m_back = translate((geometry->v1));
			m = CRAB::resizeMatrix(scale_vector.x, scale_vector.y, scale_vector.z) ;
		}

		if (ImGui::DragFloat3("Normal Reflection Plan", (float*)&reflection_normal_vector, 0.5f)) {
			//
		}

		if (ImGui::Button("Reflect"))
		{
			CRAB::Matrix4 m_to_origin = translate((geometry->v1) * -1);
			CRAB::Matrix4 m_back = translate((geometry->v1));
			if (reflection_normal_vector.length() > 0)
			{
				reflection_normal_vector = reflection_normal_vector.to_unitary();
			}
			m = m_back * reflectionArbitrary(reflection_normal_vector) * m_to_origin;
		}

		geometry->transform(m);

	}

	if (typeid(*selected_geometry) == typeid(Triangle)) {
		Triangle *geometry = (Triangle*)selected_geometry;

		CRAB::Vector4Df tranlate_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_origin_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df angle_object_vector = CRAB::Vector4Df{ 0,0,0,0 };
		CRAB::Vector4Df scale_vector = CRAB::Vector4Df{ 1,1,1,0 };
		static CRAB::Vector4Df reflection_normal_vector = CRAB::Vector4Df{ 0,0,0,0 };
		
		static CRAB::Vector4Df quaternion{ 0,1,0,0 };
		float angle_quat_vector = 0.0f;

		CRAB::Matrix4 m = CRAB::Identity();

		ImGui::DragFloat3("Translate", (float*)&tranlate_vector, 0.5f);
		m = CRAB::translate(tranlate_vector);

		if (ImGui::DragFloat3("Angle (Origin)", (float*)&angle_origin_vector, 0.5f)) {
			m = CRAB::rotateZ(angle_origin_vector.z)*CRAB::rotateY(angle_origin_vector.y)*CRAB::rotateX(angle_origin_vector.x)*m;
		}

		if (ImGui::DragFloat3("Angle (Base Vertex)", (float*)&angle_object_vector, 0.5f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->v1) * -1);
			CRAB::Matrix4 m_back = translate((geometry->v1));
			m = m_back * CRAB::rotateZ(angle_object_vector.z)*CRAB::rotateY(angle_object_vector.y)*CRAB::rotateX(angle_object_vector.x) * m_to_origin;
		}

		if (ImGui::DragFloat("Quaternion Angle (W)", (float*)&angle_quat_vector, 0.5f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->v1) * -1);
			CRAB::Matrix4 m_back = translate((geometry->v1));
			CRAB::Vector4Df quaternio_final = quaternion.to_unitary();

			m = m_back * CRAB::rotateArbitrary(angle_quat_vector, quaternio_final) * m_to_origin;
		}

		if (ImGui::DragFloat3("Quatertion Vector (V)", (float*)&quaternion, 0.1f)) {
			//
		}

		if (ImGui::DragFloat3("Scale", (float*)&scale_vector, 0.1f)) {
			CRAB::Matrix4 m_to_origin = translate((geometry->v1) * -1);
			CRAB::Matrix4 m_back = translate((geometry->v1));
			m = CRAB::resizeMatrix(scale_vector.x, scale_vector.y, scale_vector.z);
		}

		if (ImGui::DragFloat3("Normal Reflection Plan", (float*)&reflection_normal_vector, 0.5f)) {
			//
		}

		if (ImGui::Button("Reflect"))
		{
			CRAB::Matrix4 m_to_origin = translate((geometry->v1) * -1);
			CRAB::Matrix4 m_back = translate((geometry->v1));
			if (reflection_normal_vector.length() > 0)
			{
				reflection_normal_vector = reflection_normal_vector.to_unitary();
			}
			m = m_back * reflectionArbitrary(reflection_normal_vector) * m_to_origin;
		}

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
