#include "WindowController.h"
#include "RenderEngine/RenderAPI.h"
#include "RenderEngine/Camera.h"

#include "RenderEngine/OpenGLRender.h"
#include "RenderEngine/RayCastRender.h"

#include "Cylinder.h"

using namespace CRAB;

RenderAPI::VertexBuffer vbo;
std::vector<Object> objs;

const int width = 512, height = 512;
Camera cam = Camera(
	Vector4Df{ 0.0f,0.0f,5.0f,1.0f },//position
	Vector4Df{ 0.0f,0.0f,0.0f,1.0f },//lookat
	Vector4Df{ 0.0f,1.0f,0.0f,0.0f },//up
	float2{width*1.0f, height*1.0f}, //resolution
	float2{ 1.0f, 1.0f },			//dimensions
	1.0f							//near
);

RayCast rc(cam);

// display function called by glutMainLoop(), gets executed every frame 
void disp(void)
{
	RenderAPI::BufferClear();

	
	//OpenGLRender(cam);
	
	

	//Cylinder
	
	
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
            cam.Transform(rotateArbitrary(deltaY, v));
            cam.Transform(rotateY(deltaX));
        }
        else if (theButtonState == GLUT_MIDDLE_BUTTON) // Zoom
        {

        }

        if (theButtonState == GLUT_RIGHT_BUTTON) // camera move
        {

            cam.position += cam.position*(deltaY*0.01f);
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

    motion(x, y);
}

void keyboard(unsigned char key, int /*x*/, int /*y*/) {

    switch (key) {

    case(27): exit(0);
    case(' '):
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

	RenderAPI::CreateVBO(&vbo, width, height);
	
	objs.push_back(new Cylinder(2.0f, 0.5f, Vector4Df{ 0,0,0,0 }, Vector4Df{ 0,1,0,0 }));
	//CreateVBO2(&vbo, width, height);
	//start render loop
    RenderAPI::RenderLoop();

	RenderAPI::DeleteVBO(&vbo);
}

