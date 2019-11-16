//
//	Main Window class
//	Call the Graphic API to cerate the window
//	Call the events to recieve user input
//	Call the render engines for renderization
//

#include "WindowController.h"
#include "RenderEngine/RenderAPI.h"
#include "RenderEngine/Camera.h"

#include "RenderEngine/OpenGLRender.h"
#include "RenderEngine/RayCastRender.h"

#include "DataIO/LoadObj.h"

#include "Light.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "Spotlights.h"
#include "DirectionalLight.h"

#include "ImGUI.h"
#include "ObjectController.h"
#include "Menu.h"

#include "GlobalVariables.h"

#include "Octree.h"

using namespace CRAB;

//pixel buffer
RenderAPI::VertexBuffer vbo;
//list of objects
std::vector<Object> objs;
std::vector<Sphere> spheres = {
	Sphere(Vector4Df{0.0f,0.0f,0.0f,1.0f},1.0f),//1
	Sphere(Vector4Df{0.0f,1.5f,0.0f,1.0f},0.35f),//2
	Sphere(Vector4Df{0.0f,2.4f,0.0f,1.0f},0.14f),//3
	Sphere(Vector4Df{0.0f,3.0f,0.0f,1.0f},0.06f),//4
	Sphere(Vector4Df{2.0f,0.0f,2.0f,1.0f},0.5f),//5
	Sphere(Vector4Df{0.0f,0.1f,3.0f,1.0f},1.5f),//6
	Sphere(Vector4Df{2.0f,0.0f,0.0f,1.0f},1.0f),//7
	Sphere(Vector4Df{2.0f,1.5f,0.0f,1.0f},0.3f),//8
};
std::vector<std::vector<Triangle>> ObjList;

//list of lights
std::vector<Light *> lights;

//list of materials
Material* Neutral = new Material(Vector4Df{ 0.3f, 0.3f, 0.3f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, 10);
Material* Mirror = new Material(Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, 200.0f, 1.0f, 0.8f);
Material* Refract = new Material(Vector4Df{ 0.5f, 0.5f, 0.5f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, Vector4Df{ 0.2f, 0.2f, 0.2f, 0 }, 2, 0.5f, 0.5f, 1.1f); //Shininess, alpha, reflection, ior
//transparent
Material* Window_Glass = new Material(Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f }, Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f }, Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f }, 2.0f, 0.1f, 1.0f, 1.1f);
//metals
Material* Bronze = new Material(Vector4Df{ 0.2125f, 0.1275f, 0.054f, 0.0f }, Vector4Df{ 0.714f, 0.4284f, 0.18144f, 0.0f }, Vector4Df{ 0.393548f, 0.271906f, 0.166721f, 0.0f }, 25.6f);
Material* Polished_Bronze = new Material(Vector4Df{ 0.25f, 0.148f, 0.06475f, 0.0f }, Vector4Df{ 0.4f, 0.2368f, 0.1036f, 0.0f }, Vector4Df{ 0.774597f, 0.458561f, 0.200621f, 0.0f }, 76.8f, 1.0f, 0.5f);
Material* Gold = new Material(Vector4Df{ 0.24725f, 0.1995f, 0.0745f, 0.0f }, Vector4Df{ 0.75164f, 0.60648f, 0.22648f, 0.0f }, Vector4Df{ 0.628281f, 0.555802f, 0.366065f, 0.0f }, 51.2f);
Material* Polished_Gold = new Material(Vector4Df{ 0.24725f, 0.2245f, 0.0645f, 0.0f }, Vector4Df{ 0.34615f, 0.3143f, 0.0903f, 0.0f }, Vector4Df{ 0.797357f, 0.723991f, 0.208006, 0.0f }, 83.2f, 1.0f, 0.5f);
Material* Silver = new Material(Vector4Df{ 0.5f, 0.5f, 0.5f, 0.0f }, Vector4Df{ 0.50754f, 0.50754f, 0.50754f, 0.0f }, Vector4Df{ 0.508273f, 0.508273f, 0.508273f, 0.0f }, 51.2f, 1.0f, 0.5f);
//jewel
Material* Jade = new Material(Vector4Df{ 0.135f, 0.2225f, 0.1575f, 0.0f }, Vector4Df{ 0.54f, 0.89f, 0.63f, 0.0f }, Vector4Df{ 0.316228f, 0.316228f, 0.316228f, 0.0f }, 12.8f, 0.95f, 0.1f);
Material* Ruby = new Material(Vector4Df{ 0.1745f, 0.01175f, 0.01175f, 0.0f }, Vector4Df{ 0.61424f, 0.04136f, 0.04136f, 0.0f }, Vector4Df{ 0.727811f, 0.626959f, 0.626959f, 0.0f }, 76.8f, 0.55f, 0.1f, 1.77f);
Material* Emerald = new Material(Vector4Df{ 0.0215f, 0.1745f, 0.0215f, 0.0f }, Vector4Df{ 0.07568f, 0.61424f, 0.07568f, 0.0f }, Vector4Df{ 0.633f, 0.727811f, 0.633f, 0.0f }, 76.8f, 0.55f, 0.1f, 1.57f);
//plastic
Material* Black_Plastic = new Material(Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f }, Vector4Df{ 0.01f, 0.01f, 0.01f, 0.0f }, Vector4Df{ 0.5f, 0.5f, 0.5f, 0.0f }, 32.0f);
Material* Cyan_Plastic = new Material(Vector4Df{ 0.0f, 0.1f, 0.06f, 0.0f }, Vector4Df{ 0.0f, 0.50980392f, 0.50980392f, 0.0f }, Vector4Df{ 0.50196078f, 0.50196078f, 0.50196078f, 0.0f }, 32.0f);
Material* Green_Plastic = new Material(Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f }, Vector4Df{ 0.1f, 0.35f, 0.1f, 0.0f }, Vector4Df{ 0.45f, 0.55f, 0.45f, 0.0f }, 32.0f);
Material* Red_Plastic = new Material(Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f }, Vector4Df{ 0.5f, 0.0f, 0.0f, 0.0f }, Vector4Df{ 0.7f, 0.6f, 0.6f, 0.0f }, 32.0f);
Material* White_Plastic = new Material(Vector4Df{ 0.4f, 0.4f, 0.4f, 0.0f }, Vector4Df{ 0.6f, 0.6f, 0.6f, 0.0f }, Vector4Df{ 0.7f, 0.7f, 0.7f, 0.0f }, 32.0f);
Material* Yellow_Plastic = new Material(Vector4Df{ 0.2f, 0.2f, 0.2f, 0.0f }, Vector4Df{ 0.6f, 0.6f, 0.0f, 0.0f }, Vector4Df{ 0.7f, 0.7f, 0.6f, 0.0f }, 32.0f);

const int	width  = 512,
			height = 512;

//main camera
Camera cam = Camera(
	//Vector4Df{ 15.0f,10.0f,30.0f,1.0f },//position
	//Vector4Df{ 10.0f, 9.0f,10.0f,1.0f },//lookat
	//Vector4Df{ 10.0f,5.0f,50.0f,1.0f },//position
	//Vector4Df{ 10.0f, 5.0f,15.0f,1.0f },//lookat
	Vector4Df{ 0.0f,0.0f,15.0f,1.0f },//position
	Vector4Df{ 0.0f, 0.0f,0.0f,1.0f },//lookat
	Vector4Df{ 0.0f,1.0f,0.0f,0.0f },//up
	float2{ width*1.0f, height*1.0f }, //resolution
	float2{ 2.0f, 2.0f },			//dimensions
	2.0f							//near
);

//raycast class for renderization 
RayCast rc(cam);


//Aux
bool main_menu_enable = 0;

//FPS couting window
void FPS_display()
{
	ImGui::Begin("FPS");

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}


//Main Menu Windows
void Main_Menu()
{
	//Light Menu Parameters
	static bool * selected = new bool [lights.size()];
	static string CurrentLightName = " ";
	static string CurrentLightType = " ";
	static int CurrentLight;

	//Camera Menu Parameters
	static const char* current_viewpoint = NULL;
	const char* items[] = { "Top", "Front", "Right", "ISO" };
	static const char* current_item = NULL;
	//Vector4Df eye_pos = Vector4Df{ 0.0f,0.0f,10.0f,1.0f };

	//Menu
	//Menu MainMenu = Menu("Main Menu");
	if (main_menu_enable)
	{
		ImGui::Begin("Main Menu");

		if (ImGui::TreeNode("Lights")){
			if (ImGui::BeginCombo("Lights", CurrentLightName.c_str()))
			{
				for (int i = 0; i < lights.size(); i++)
				{
					if (ImGui::Selectable(("Light " + to_string(i)).c_str(), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
						CurrentLightName = ("Light " + to_string(i)).c_str();
						CurrentLight = i;
					}
				}
				ImGui::EndCombo();
			}
			if (typeid(*lights[CurrentLight]) == typeid(AmbientLight))
			{
				CurrentLightType = "Ambient Light";
				ImGui::DragFloat3("Intensity", (float*)&lights[CurrentLight]->intensity, 0.1f, 0.0f, 1.0f);
				ImGui::Text(CurrentLightType.c_str());
			}else if (typeid(*lights[CurrentLight]) == typeid(DirectionalLight))
			{
				CurrentLightType = "Directional Light";
				ImGui::Text(CurrentLightType.c_str());
				ImGui::DragFloat3("Intensity", (float*)&lights[CurrentLight]->intensity, 0.1f, 0.0f, 1.0f);
				if (ImGui::DragFloat3("Direction", (float*)&((DirectionalLight *)lights[CurrentLight])->direction, 0.1f)) {
					((DirectionalLight *)lights[CurrentLight])->direction.normalize();
				}
				ImGui::Text(CurrentLightType.c_str());
			}
			else if (typeid(*lights[CurrentLight]) == typeid(Spotlights))
			{
				CurrentLightType = "Spot Light";
				ImGui::DragFloat3("Intensity", (float*)&lights[CurrentLight]->intensity, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat3("Position", (float*)&((Spotlights *)lights[CurrentLight])->position, 0.5f);
				if (ImGui::DragFloat3("Direction", (float*)&((Spotlights *)lights[CurrentLight])->direction, 0.1f)) {
					((Spotlights *)lights[CurrentLight])->direction.normalize();
				}
				ImGui::DragFloat("Angle", (float*)&((Spotlights *)lights[CurrentLight])->angle, 0.5f, 0.0f, 180.0f);
				ImGui::Text(CurrentLightType.c_str());
			}
			else if (typeid(*lights[CurrentLight]) == typeid(PointLight))
			{
				CurrentLightType = "Point Light";
				ImGui::DragFloat3("Intensity", (float*)&lights[CurrentLight]->intensity, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat3("Position", (float*)&((PointLight *)lights[CurrentLight])->position, 0.5f);
				ImGui::Text(CurrentLightType.c_str());
			}
			else
			{
				CurrentLightType = "None";
				ImGui::Text(CurrentLightType.c_str());
			}
		
			ImGui::Checkbox("On", &(lights[CurrentLight]->on));


			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Projections")) {

			ImGui::RadioButton("Perspective", &graphicalProjection, 1);
			ImGui::RadioButton("Orthographic", &graphicalProjection, 2);
			ImGui::DragFloat2("Screen size", (float*) & (cam.dimensions), 1.0f, 2.0f, 100.0f, "%.1f");
			ImGui::RadioButton("Oblique", &graphicalProjection, 3);
			ImGui::SliderFloat("Angle-X (degree)", &obliqueAngleX, 15.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Angle-Y (degree)", &obliqueAngleY, 15.0f, 60.0f, "%.1f");

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Camera")) {

			if (ImGui::BeginCombo("Viewpoint", current_viewpoint))
			{
				for (int i = 0; i < IM_ARRAYSIZE(items); i++)
				{
					bool is_selected = (current_item == items[i]);
					if (ImGui::Selectable(items[i], is_selected)) {
						current_item = items[i];
						current_viewpoint = items[i];
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			if (current_item == items[0]) // Top view
			{
				cam.position = { 0.0f, 30.0f, 0.0f, 1.0f };
				cam.view = { 0.0f, 0.0f, 0.0f, 1.0f };
				cam.up = { 0.0f, 0.0f, -1.0f, 0.0f };
				current_item = NULL;
			}
			else if (current_item == items[1]) // Front view
			{
				cam.position = { 0.0f, 0.0f, 30.0f, 1.0f };
				cam.view = { 0.0f, 0.0f, 0.0f, 1.0f };
				cam.up = { 0.0f, 1.0f, 0.0f, 0.0f };
				current_item = NULL;
			}
			else if (current_item == items[2]) // Right view
			{
				cam.position = { 30.0f, 0.0f, 0.0f, 1.0f };
				cam.view = { 0.0f, 0.0f, 0.0f, 1.0f };
				cam.up = { 0.0f, 1.0f, 0.0f, 0.0f };
				current_item = NULL;
			}
			else if (current_item == items[3]) // ISO view
			{
				cam.position = { 20.0f, 20.0f, 20.0f, 1.0f };
				cam.view = { 0.0f, 0.0f, 0.0f, 1.0f };
				cam.up = { 0.0f, 1.0f, 0.0f, 0.0f };
				cam.NewViewUp();
				current_item = NULL;
			}
		
			ImGui::DragFloat3("Eye Position", (float*)&(cam.position), 0.1f);
			ImGui::DragFloat3("Look At", (float*)&(cam.view), 0.1f);
			cam.NewViewUp();
			ImGui::DragFloat3("View Up", (float*)&(cam.up), 0.1f, 0.0f, 1.0f);

			ImGui::TreePop();
		}

		ImGui::End();
	}
	//MainMenu.CloseMenu();
}

// display function called by MainLoop(), gets executed every frame 
void disp(void)
{
	// Start the Dear ImGui frame
	GUI_API::StartFrame();

	RenderAPI::BufferClear();


	//Render using RayCast
	Vector4Df* colorBuffer = rc.Render(cam, objs, lights);
	//Vector4Df* colorBuffer = rc.Render(cam, spheres);
	RenderAPI::BufferBind(vbo);
	RenderAPI::MapBuffer(colorBuffer, width, height, vbo);

	//GUI displays
	FPS_display();
	Main_Menu();
	RenderObject();

	//GUI rendering call
	GUI_API::RenderFrame();

	//End frame
	RenderAPI::SwapBuffers();
	RenderAPI::ReDisplay();
}

// mouse event handlers
int lastX = 0, lastY = 0;
int theButtonState = 0;
int theModifierState = 0;

// camera mouse controls in X and Y direction
void motion(int x, int y)
{
	ImGui_ImplGLUT_MotionFunc(x, y);

	int deltaX = lastX - x;
	int deltaY = lastY - y;

	if (!(ImGui::GetIO().WantCaptureMouse)) {
		if (deltaX != 0 || deltaY != 0) {

			if (theButtonState == GLUT_LEFT_BUTTON)  // Rotate
			{
				Vector4Df v = cross(cam.view - cam.position, cam.up);
				cam.Transform(rotateArbitrary(-deltaY, v), cam.view);
				cam.Transform(rotateY(deltaX), cam.view);
			}
			else if (theButtonState == GLUT_MIDDLE_BUTTON) // Zoom
			{

			}

			if (theButtonState == GLUT_RIGHT_BUTTON) // camera move
			{

				cam.position = cam.position + (cam.position - cam.view)*(deltaY*0.01f);
				//float2{ cam.fov.x * (1+deltaY*0.1f), cam.fov.y * (1 + (deltaY*0.1f)) };
			//interactiveCamera->changeRadius(-deltaY * 0.01);
			}

			lastX = x;
			lastY = y;
		}
	}
}

void mouse(int button, int state, int x, int y)
{
	ImGui_ImplGLUT_MouseFunc(button, state, x, y);

	if (!(ImGui::GetIO().WantCaptureMouse)) {
		theButtonState = button;
		theModifierState = glutGetModifiers();
		lastX = x;
		lastY = y;

		if (theButtonState == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) // camera move
		{
			selected_obj = nullptr;
			selected_obj = rc.RayPick(cam, objs, x, y);
			if (selected_obj) {
				object_window_open = true;
			}

		}

		motion(x, y);
	}
}

void keyboard(unsigned char key, int x, int y) {
	ImGui_ImplGLUT_KeyboardFunc(key, x, y);
	Object* o;
	switch (key) {

	case(27): exit(0);
	case(' '):
		break;
	case('t'):
		int x, y;
		std::cout << "x\n";
		std::cin >> x;
		std::cout << "y\n";
		std::cin >> y;
		o = rc.RayPick(cam, objs, x, y);
		if (o)o->visible = true;
		break;
	
	case('m'):
		
		if (reflection_en)
		{
			std::cout << "Reflection is disable now!\n";
			reflection_en = 0;
		}
		else
		{
			std::cout << "Reflection is enable! Be careful.\n";
			reflection_en = 1;
		}
		
		break;

	case('s'):
	case('S'):
		if (main_menu_enable)
		{
			main_menu_enable = 0;
		}
		else {
			main_menu_enable = 1;
		}
		break;

	case('l'):
	case('L'):
		std::cout << "Load OBJ File:" << std::endl;
		std::string fileName;
		std::cin >> fileName;
		std::vector<FaceList> faceList = CRAB::Load_Obj(fileName);
		objs.clear();
		
		for (int i = 0; i < faceList.size(); i++) {
			objs.push_back(Object("OBJ", Neutral, new OcTree(faceList[i])));
		}
		//for (int i = 0; i < ObjList.size(); i++)
		//	for (int j = 0; j < ObjList[i].size(); j++)
		//		objs.push_back(Object("OBJ", Neutral, &ObjList[i][j]));
		break;
	
	}
}

void resize(int w, int h) {
	ImGui_ImplGLUT_ReshapeFunc(w, h);
	RenderAPI::Reshape(w, h);
}

void InitScene() {
	//fill the light list
	//lights.push_back(new AmbientLight(Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }));
	//lights.push_back(new Spotlights(Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, Vector4Df{ 0.0f, 0, -30.0f, 1 }, Vector4Df{ 0.0f, 0.0f, -1.0f, 0 }, 20.0f, 50.0f));
	//lights.push_back(new Spotlights(Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, Vector4Df{ 0.0f, 3.0f, 0.0f, 1 }, Vector4Df{ 0.0f, -1.0f, 0.0f, 0 }, 50.0f, 10.0f));
	lights.push_back(new DirectionalLight(Vector4Df{ 1.0f, 1.0f, 1.0f, 0.0f }, Vector4Df{ 0.2f, 0.2f, 1.0f, 0.0f }));
	lights.push_back(new DirectionalLight(Vector4Df{ 0.6f, 0.6f, 0.6f, 0.0f }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0.0f }));
	//lights.push_back(new Spotlights(Vector4Df{ 1.0f, 1.0f, 1.0f, 0.0f }, Vector4Df{ 3.0f, 1.0f, -4.5f, 1.0f }, Vector4Df{ 1.0f, 0.0f, 1.0f, 0.0f }, 5.0f, 50.0f));
	//lights.push_back(new PointLight(Vector4Df{ 1.0f, 1.0f, 1.0f, 0}, Vector4Df{ 5.0f, 7.0f, 30.0f, 1 }));

	//fill the object list
	//objs.push_back(Object(Vector4Df{ 0.4f, 0.2f, 0.1f,0 }, new Cylinder(4.0f, 0.5f, Vector4Df{ 0,0,-10,1 }, Vector4Df{ 0,1,0,0 })));
	//objs.push_back(Object(Vector4Df{ 0.0f, 1.0f, 0.0f,0 }, new Cone(9.0f, 2.0f, Vector4Df{ 0,4,-10,1 }, Vector4Df{ 0,1,0,0 })));
	//
	//objs.push_back(Object(Vector4Df{ 1.0f, 0.5f, 1.0f,0 }, new Cube(Vector4Df{ 0,0, -20,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 5.0f)));
	//objs.push_back(Object(Vector4Df{ 1.0f, 0.5f, 1.0f,0 }, new Cube(Vector4Df{ 0,5, -20,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 5.0f)));
	//objs.push_back(Object(Vector4Df{ 1.0f, 0.5f, 1.0f,0 }, new Cube(Vector4Df{ 0,10,-20,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 5.0f)));
	//
	//// QUAD
	//objs.push_back(Object(Vector4Df{ 0.1f, 0.2f, 0.2f,0 }, new Quad({ 3.5f, -3.0f, 0.0f, 1.0f }, { -3.5f, -3.0f, 0.0f, 1.0f }, { -3.5f, 0.0f, 0.0f, 1.0f }, { 3.5f, 0.0f, 0.0f, 1.0f })));
	////tin floor
	//objs.push_back(Object(Vector4Df{ 0.1f, 0.2f, 0.2f,0 }, new Quad({ 3.5f, -3.0f, 0.0f, 1.0f }, { -3.5f, -3.0f, 0.0f, 1.0f }, { -3.5f, -3.0f, 5.0f, 1.0f }, { 3.5f, -3.0f, 5.0f, 1.0f })));
	////floor
	//objs.push_back(Object(Vector4Df{ 0.1f, 0.2f, 0.2f,0 }, new Quad({ 3.5f, 0.0f, -30.0f, 1.0f }, { -3.5f, 0.0f, -30.0f, 1.0f }, { -3.5f, 0.0f, 0.0f, 1.0f }, { 3.5f, 0.0f, 0.0f, 1.0f })));

	Material *Verde = new Material(Vector4Df{ 0.0f, 0.1f, 0.0f, 0 }, Vector4Df{ 0.0f, 1.0f, 0.0f, 0 }, Vector4Df{ 0.0f, 1.0f, 0.0f, 0 }, 1000);
	Material *Tronco = new Material(Vector4Df{ 0.04f, 0.02f, 0.01f, 0 }, Vector4Df{ 0.4f, 0.2f, 0.1f, 0 }, Vector4Df{ 0.4f, 0.2f, 0.1f, 0 }, 1000);
	Material *Parede = new Material(Vector4Df{ 0.04f, 0.03f, 0.02f, 0 }, Vector4Df{ 0.8f, 0.6f, 0.4f, 0 }, Vector4Df{ 0.8f, 0.6f, 0.4f, 0 }, 500);
	Material *Parede2 = new Material(Vector4Df{ 0.04f, 0.03f, 0.02f, 0 }, Vector4Df{ 0.6f, 0.8f, 0.1f, 0 }, Vector4Df{ 0.6f, 0.8f, 0.1f, 0 }, 500);
	Material *Parede3 = new Material(Vector4Df{ 0.04f, 0.03f, 0.02f, 0 }, Vector4Df{ 0.3f, 0.6f, 0.4f, 0 }, Vector4Df{ 0.3f, 0.6f, 0.4f, 0 }, 500);

	//fill the object list
	//objs.push_back(Object("Tronco da arvore 1", Tronco, new Cylinder(2.0f, 0.5f, Vector4Df{ 5.0f,0,30,1 }, Vector4Df{ 0,1,0,0 })));
	//objs.push_back(Object("Copa da arvode 1", Verde, new Cone(8.0f, 3.0f, Vector4Df{ 5.0f,2,30,1 }, Vector4Df{ 0,1,0,0 })));
	
	//objs.push_back(Object("Tronco da arvore 2", Tronco, new Cylinder(2.0f, 0.5f, Vector4Df{ 15,0,30,1 }, Vector4Df{ 0,1,0,0 })));
	//objs.push_back(Object("Copa da arvore 2", Verde, new Cone(8.0f, 3.0f, Vector4Df{ 15,2,30,1 }, Vector4Df{ 0,1,0,0 })));
	
	//objs.push_back(Object("Copa da arvore 2.2", Verde, new Cone(8.0f, 3.0f, Vector4Df{ 15,2,30,1 }, Vector4Df{ 0,1,0,0 })));


	//objs.push_back(Object("Cubo 1", Parede, new Cube(Vector4Df{ 10, 0, 10,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 6.0f)));
	//objs.push_back(Object("Cubo 2", Parede2, new Cube(Vector4Df{ 10, 6, 10,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 6.0f)));
	//objs.push_back(Object("Cubo 3", Parede3, new Cube(Vector4Df{ 10,12, 10,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 6.0f)));

	//objs.push_back(Object("plane", Parede3, new Triangle(Vector4Df{ -10,0, -20,1 }, Vector4Df{ 10,0,-20,1 }, Vector4Df{ 0,10,-20,1 })));
	//objs.push_back(Object("plane2", Parede3, new Triangle(Vector4Df{ -10,0, 0,1 }, Vector4Df{ 10,0,0,1 }, Vector4Df{ 0,10,0,1 })));

	//Fill the object list (With material)
	//objs.push_back(Object("Tronco da arvore 1", new Material(Vector4Df{ 0.4f, 0.2f, 0.1f, 0 }, Vector4Df{ 0.0f, 0.0f, 0.0f, 0 }, Vector4Df{ 0.0f, 0.0f, 0.0, 0 }, 0.5f), new Cylinder(2.0f, 0.5f, Vector4Df{ 5.0f,0,30,1 }, Vector4Df{ 0,1,0,0 })));
	//objs.push_back(Object("Cilindro", new Material(Vector4Df{ 0.19225f, 0.19225f, 0.19225f, 0 }, Vector4Df{ 0.50754f, 0.50754f, 0.50754f, 0 }, Vector4Df{ 0.508273f, 0.508273f, 0.508273f, 0 }, 5.0f), new Cylinder(2.0f, 0.5f, Vector4Df{ 0.0f,0,0,1 }, Vector4Df{ 0,1,0,0 })));
	//objs.push_back(Object("Esfera", new Material(Vector4Df{ 0.19225f, 0.19225f, 0.19225f, 0 }, Vector4Df{ 0.50754f, 0.50754f, 0.50754f, 0 }, Vector4Df{ 0.508273f, 0.508273f, 0.508273f, 0 }, 5.0f), new Sphere(Vector4Df{ 0.0f, 0.0f, 0.0f, 1 }, 2.0f)));
	//objs.push_back(Object("Cone", new Material(Vector4Df{ 0.19225f, 0.19225f, 0.19225f, 0 }, Vector4Df{ 0.50754f, 0.50754f, 0.50754f, 0 }, Vector4Df{ 0.508273f, 0.508273f, 0.508273f, 0 }, 5.0f), new Cone(6.0f, 2.0f, Vector4Df{ 0.0f,0,0,1 }, Vector4Df{ 0,1,0,0 })));
	//objs.push_back(Object("Cubo 3", new Material(Vector4Df{ 0.19225f, 0.19225f, 0.19225f, 0 }, Vector4Df{ 0.50754f, 0.50754f, 0.50754f, 0 }, Vector4Df{ 0.508273f, 0.508273f, 0.508273f, 0 }, 5.0f), new Cube(Vector4Df{ 0, 0, 0,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 2.0f)));
	//objs.push_back(Object("Quad", new Material(Vector4Df{ 0.19225f, 0.19225f, 0.19225f, 0 }, Vector4Df{ 0.50754f, 0.50754f, 0.50754f, 0 }, Vector4Df{ 0.508273f, 0.508273f, 0.508273f, 0 }, 5.0f), new Quad(Vector4Df{ 0, 0, -3,1 }, Vector4Df{ 2,0,-3,0 }, Vector4Df{ 2,2,-3,0 }, Vector4Df{ 0,2,-3,0 })));
	//objs.push_back(Object("Refract Cube", new Material(Vector4Df{ 0.19225f, 0.19225f, 0.19225f, 0 }, Vector4Df{ 0.50754f, 0.50754f, 0.50754f, 0 }, Vector4Df{ 0.508273f, 0.508273f, 0.508273f, 0 }, 1000, 1, 0.8f, 1.33f), new Cube(Vector4Df{ 0, 0, 6,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 2.0f)));
	objs.push_back(Object("Refract Sphere", Refract, new Sphere(Vector4Df{ 0.0f, 1.0f, 6.0f, 1 }, 2.0f)));
	//objs.push_back(Object("Refract Cylinder", Refract, new Cylinder(2.0f, 0.5f, Vector4Df{ 0.0f,0,6.0f,1 }, Vector4Df{ 0,1,0,0 })));
	//Material* Mirror = new Material(Vector4Df{ 0.3f, 0.3f, 0.3f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, 1000, 1, 0.8f);
	objs.push_back(Object("Cube", Neutral, new Cube(Vector4Df{ 0.0f, 0.0f, 0.0f, 1.0f }, Vector4Df{ 0.0f, 1.0f, 0.0f, 0.0f }, Vector4Df{ 0.0f, 0.0f, 1.0f, 0.0f }, 2.0f)));
	objs.push_back(Object("silver tray", Silver, new Quad(Vector4Df{ 5.0f, -1.0f, 5.0f, 1.0f }, Vector4Df{ 5.0f, -1.0f, -5.0f, 1.0f }, Vector4Df{ -5.0f, -1.0f, -5.0f, 1.0f }, Vector4Df{ -5.0f, -1.0f, 5.0f, 1.0f })));
	objs.push_back(Object("Sphere", Yellow_Plastic, new Sphere(Vector4Df{ 0.0f, 1.0f, -6.0f, 1.0f }, 2.0f)));
	objs.push_back(Object("Sphere", Cyan_Plastic, new Sphere(Vector4Df{ 3.0f, 1.0f, -6.0f, 1.0f }, 1.5f)));
}

// Main.
void Start_Window(int argc, char **argv) {
	// Create GLUT window
	RenderAPI::StartRenderAPI(argc, argv, width, height);
	
	// functions for user interaction
	RenderAPI::MouseFunc(mouse);
	RenderAPI::MotionFunc(motion);
	RenderAPI::KeyboardFunc(keyboard);
	RenderAPI::ReshapeFunc(resize);
	
	RenderAPI::DisplayFunc(disp);

	//create the pixel buffer
	RenderAPI::CreateVBO(&vbo, width, height);
	
	GUI_API::Start_ImGUI();

	//Fill the scene
	InitScene();
	/*
	std::vector<FaceList> faceList = CRAB::Load_Obj("crab3.obj");
	//objs.clear();
	for (int i = 0; i < faceList.size(); i++) {
		objs.push_back(Object("OBJ", Neutral, new OcTree(faceList[i])));
	}
	std::vector<FaceList> faceList2 = CRAB::Load_Obj("wall.obj");
	for (int i = 0; i < faceList2.size(); i++) {
		objs.push_back(Object("OBJ", Mirror, new OcTree(faceList2[i])));
	}*/
	
	//start render loop
	RenderAPI::RenderLoop();

	GUI_API::EndImGUI();
	//delete the pixel buffer
	RenderAPI::DeleteVBO(&vbo);
}

