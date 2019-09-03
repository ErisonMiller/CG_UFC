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

#include "Cylinder.h"
#include "Quad.h"
#include "Cube.h"
#include "Cone.h"
#include "Sphere.h"
#include "Triangle.h"

using namespace CRAB;

//pixel buffer
RenderAPI::VertexBuffer vbo;
//list of objects
std::vector<Object> objs;

const int width = 512, height = 512;

//main camera
Camera cam = Camera(
	//Vector4Df{ 15.0f,10.0f,30.0f,1.0f },//position
	//Vector4Df{ 10.0f, 9.0f,10.0f,1.0f },//lookat
	Vector4Df{ 10.0f,5.0f,50.0f,1.0f },//position
	Vector4Df{ 10.0f, 5.0f,15.0f,1.0f },//lookat
	Vector4Df{ 0.0f,1.0f,0.0f,0.0f },//up
	float2{width*1.0f, height*1.0f}, //resolution
	float2{ 4.0f, 4.0f },			//dimensions
	4.0f							//near
);

//raycast class for renderization 
RayCast rc(cam);

// display function called by MainLoop(), gets executed every frame 
void disp(void)
{
	RenderAPI::BufferClear();

	//Render using OpenGL
	//OpenGLRender(cam);
	
	

	//Render using RayCast
	RenderAPI::BufferBind(vbo);
	Vector4Df* colorBuffer = rc.Render(cam, objs);
	RenderAPI::MapBuffer(colorBuffer, width, height);
	

	RenderAPI::SwapBuffers();

}

// mouse event handlers

int lastX = 0, lastY = 0;
int theButtonState = 0;
int theModifierState = 0;

// camera mouse controls in X and Y direction
void motion(int x, int y)
{
    int deltaX = lastX - x;
    int deltaY = lastY - y;

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

            cam.position = cam.position + (cam.position-cam.view)*(deltaY*0.01f);
            //float2{ cam.fov.x * (1+deltaY*0.1f), cam.fov.y * (1 + (deltaY*0.1f)) };
        //interactiveCamera->changeRadius(-deltaY * 0.01);
        }

        lastX = x;
        lastY = y;
        glutPostRedisplay();

    }
}

void mouse(int button, int state, int x, int y)
{
    theButtonState = button;
    theModifierState = glutGetModifiers();
    lastX = x;
    lastY = y;

	if (theButtonState == GLUT_RIGHT_BUTTON) // camera move
	{
		Object *o = rc.RayPick(cam, objs, x, y);
		if (o)o->visible = true;
	}

	glutPostRedisplay();
    motion(x, y);
}

void keyboard(unsigned char key, int /*x*/, int /*y*/) {

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
    glutPostRedisplay();
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
	objs.push_back(Object("Tronco da arvore 1", Vector4Df{ 0.4f, 0.2f, 0.1f, 0 }, new Cylinder(2.0f, 0.5f, Vector4Df{ 5.0f,0,30,1 }, Vector4Df{ 0,1,0,0 })));
	objs.push_back(Object("Copa da arvode 1", Vector4Df{ 0.0f, 1.0f, 0.0f, 0 }, new Cone(8.0f, 3.0f, Vector4Df{ 5.0f,2,30,1 }, Vector4Df{ 0,1,0,0 })));
	
	objs.push_back(Object("Tronco da arvore 2", Vector4Df{ 0.4f, 0.2f, 0.1f, 0 }, new Cylinder(2.0f, 0.5f, Vector4Df{ 15,0,30,1 }, Vector4Df{ 0,1,0,0 })));
	objs.push_back(Object("Copa da arvore 2", Vector4Df{ 0.0f, 1.0f, 0.0f, 0 }, new Cone(8.0f, 3.0f, Vector4Df{ 15,2,30,1 }, Vector4Df{ 0,1,0,0 })));

	objs.push_back(Object("Cubo 1", Vector4Df{ 1.0f, 0.6f, 0.1f, 0 }, new Cube(Vector4Df{ 10, 0, 10,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 6.0f)));
	objs.push_back(Object("Cubo 2", Vector4Df{ 0.8f, 0.8f, 0.3f, 0 }, new Cube(Vector4Df{ 10, 6, 10,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 6.0f)));
	objs.push_back(Object("Cubo 3", Vector4Df{ 0.6f, 0.4f, 0.5f, 0 }, new Cube(Vector4Df{ 10,12, 10,1 }, Vector4Df{ 0,1,0,0 }, Vector4Df{ 0,0,1,0 }, 6.0f)));

	//start render loop
    RenderAPI::RenderLoop();

	//delete the pixel buffer
	RenderAPI::DeleteVBO(&vbo);
}

