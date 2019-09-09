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

#include "Light.h"
#include "AmbientLight.h"
#include "Spotlights.h"
#include "DirectionalLight.h"

#include "ImGUI.h"
#include "ObjectController.h"

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

//list of lights
std::vector<Light *> lights;

const int width = 512, height = 512;

//main camera
Camera cam = Camera(
	//Vector4Df{ 15.0f,10.0f,30.0f,1.0f },//position
	//Vector4Df{ 10.0f, 9.0f,10.0f,1.0f },//lookat
	//Vector4Df{ 10.0f,5.0f,50.0f,1.0f },//position
	//Vector4Df{ 10.0f, 5.0f,15.0f,1.0f },//lookat
	Vector4Df{ 0.0f,0.0f,10.0f,1.0f },//position
	Vector4Df{ 0.0f, 0.0f,0.0f,1.0f },//lookat
	Vector4Df{ 0.0f,1.0f,0.0f,0.0f },//up
	float2{ width*1.0f, height*1.0f }, //resolution
	float2{ 4.0f, 4.0f },			//dimensions
	4.0f							//near
);

//raycast class for renderization 
RayCast rc(cam);

// Our state
//static bool show_demo_window = false;
static bool show_another_window = false;

void my_display_code()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (show_demo_window)
	//	ImGui::ShowDemoWindow(&show_demo_window);

	//menu();

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world! opções");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text. opções");               // Display some text (you can use a format strings too)
//		ImGui::Checkbox("Demo Window opções", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f


		if (ImGui::Button("Play animation")) {                            // Buttons return true when clicked (most widgets return true when edited/activated)
		}
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

// display function called by MainLoop(), gets executed every frame 
void disp(void)
{
	// Start the Dear ImGui frame
	GUI_API::StartFrame();

	RenderAPI::BufferClear();

	//Render using OpenGL
	//OpenGLRender(cam);


	//Render using RayCast
	RenderAPI::BufferBind(vbo);
	Vector4Df* colorBuffer = rc.Render(cam, objs, lights);
	//Vector4Df* colorBuffer = rc.Render(cam, spheres);
	RenderAPI::MapBuffer(colorBuffer, width, height);


	my_display_code();
	RenderObject();

	GUI_API::RenderFrame();

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
				cam.Transform(rotateArbitrary(deltaY, v), cam.view);
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

		if (theButtonState == GLUT_RIGHT_BUTTON) // camera move
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
		Object* o = rc.RayPick(cam, objs, x, y);
		if (o)o->visible = true;
		break;
	}
}

// Main.
void Start_Window(int argc, char **argv) {
    // Create GLUT window
	RenderAPI::StartRenderAPI(argc, argv, width, height);
    
	// functions for user interaction
    RenderAPI::MouseFunc(mouse);
    RenderAPI::MotionFunc(motion);
    RenderAPI::KeyboardFunc(keyboard);
	
	RenderAPI::DisplayFunc(disp);

	//create the pixel buffer
	RenderAPI::CreateVBO(&vbo, width, height);
	GUI_API::Start_ImGUI();

	//fill the light list
	lights.push_back(new AmbientLight(Vector4Df{ 1.0f, 1.0f, 1.0f,0 }));
	//lights.push_back(new Spotlights(Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, Vector4Df{ 7.0f, 1.5f, 30.0f, 1 }, Vector4Df{ -1.0f, 0.0f, 0.0f, 0 }, 20.0f, 50.0f));
	lights.push_back(new DirectionalLight(Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, Vector4Df{ 0, 0, 1.0f, 0 }));
	//lights.push_back(new Spotlights(Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, Vector4Df{ 5.0f, 7.0f, 33.0f, 1 }, Vector4Df{ 0.0f, -1.0f, -1.0f, 0 }, 30.0f, 30.0f));
	
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

	//fill the object list
	//objs.push_back(Object("Tronco da arvore 1", Vector4Df{ 0.4f, 0.2f, 0.1f, 0 }, new Cylinder(2.0f, 0.5f, Vector4Df{ 5.0f,0,30,1 }, Vector4Df{ 0,1,0,0 })));
	//objs.push_back(Object("Copa da arvode 1", Vector4Df{ 0.0f, 1.0f, 0.0f, 0 }, new Cone(8.0f, 3.0f, Vector4Df{ 5.0f,2,30,1 }, Vector4Df{ 0,1,0,0 })));
	//
	//objs.push_back(Object("Tronco da arvore 2", Vector4Df{ 0.4f, 0.2f, 0.1f, 0 }, new Cylinder(2.0f, 0.5f, Vector4Df{ 15,0,30,1 }, Vector4Df{ 0,1,0,0 })));
	//objs.push_back(Object("Copa da arvore 2", Vector4Df{ 0.0f, 1.0f, 0.0f, 0 }, new Cone(8.0f, 3.0f, Vector4Df{ 15,2,30,1 }, Vector4Df{ 0,1,0,0 })));
	//
	//objs.push_back(Object("Cubo 1", Vector4Df{ 1.0f, 0.6f, 0.1f, 0 }, new Cube(Vector4Df{ 10, 0, 10,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 6.0f)));
	//objs.push_back(Object("Cubo 2", Vector4Df{ 0.8f, 0.8f, 0.3f, 0 }, new Cube(Vector4Df{ 10, 6, 10,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 6.0f)));
	//objs.push_back(Object("Cubo 3", Vector4Df{ 0.6f, 0.4f, 0.5f, 0 }, new Cube(Vector4Df{ 10,12, 10,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 6.0f)));

	//Fill the object list (With material)
	/*objs.push_back(Object("Tronco da arvore 1", new Material(Vector4Df{ 0.4f, 0.2f, 0.1f, 0 }, Vector4Df{ 0.0f, 0.0f, 0.0f, 0 }, Vector4Df{ 0.0f, 0.0f, 0.0, 0 }), new Cylinder(2.0f, 0.5f, Vector4Df{ 5.0f,0,30,1 }, Vector4Df{ 0,1,0,0 })));*/
	//objs.push_back(Object("Cilindro", new Material(Vector4Df{ 0.19225f, 0.19225f, 0.19225f, 0 }, Vector4Df{ 0.50754f, 0.50754f, 0.50754f, 0 }, Vector4Df{ 0.508273f, 0.508273f, 0.508273f, 0 }, 5.0f), new Cylinder(2.0f, 0.5f, Vector4Df{ 5.0f,0,30,1 }, Vector4Df{ 0,1,0,0 })));
	//objs.push_back(Object("Esfera", new Material(Vector4Df{ 0.19225f, 0.19225f, 0.19225f, 0 }, Vector4Df{ 0.50754f, 0.50754f, 0.50754f, 0 }, Vector4Df{ 0.508273f, 0.508273f, 0.508273f, 0 }, 15.0f), new Sphere(Vector4Df{ 0.0f, 0.0f, 0.0f, 1 }, 2.0f)));
	objs.push_back(Object("Cone", new Material(Vector4Df{ 0.19225f, 0.19225f, 0.19225f, 0 }, Vector4Df{ 0.50754f, 0.50754f, 0.50754f, 0 }, Vector4Df{ 0.508273f, 0.508273f, 0.508273f, 0 }, 15.0f), new Cone(6.0f, 2.0f, Vector4Df{ 0.0f,0,0,1 }, Vector4Df{ 0,1,0,0 })));



	//start render loop
    RenderAPI::RenderLoop();

	GUI_API::EndImGUI();
	//delete the pixel buffer
	RenderAPI::DeleteVBO(&vbo);
}

