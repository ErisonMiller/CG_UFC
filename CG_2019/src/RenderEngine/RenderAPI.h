#pragma once

//
//	This is to make a bridge btw the code and the grapgic API
//	In this file we are using the OpenGL API, but can be easily changed without changing the main code
//


#include <GL/glew.h>
#include <GL/glut.h>

#include "Linear_Algebra.h"

namespace RenderAPI{

	typedef GLuint VertexBuffer;


	inline void StartRenderAPI(int argc, char **argv, int width, int height)
	{
		// init glut:
		glutInit(&argc, argv);
		// specify the display mode to be RGB and single buffering:
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
		// specify the initial window position:
		glutInitWindowPosition(500, 100);
		// specify the initial window size:
		glutInitWindowSize(width, height);
		// create the window and set title:
		glutCreateWindow("CG");
		
		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);

		glClearColor(0.0, 1.0, 0.0, 0.0);
		glMatrixMode(GL_PROJECTION);
		gluOrtho2D(0.0, width, 0.0, height);

		glewInit();
		if (!glewIsSupported("GL_VERSION_2_0 ")) {
			exit(0);
		}
	}

	inline void BufferClear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	inline void RenderLoop()
	{
		glutMainLoop();
	}

	inline void CreateVBO(VertexBuffer *_vbo, int width, int height)
	{
		//Create vertex buffer object
		glGenBuffers(1, _vbo);
		glBindBuffer(GL_ARRAY_BUFFER, *_vbo);

		//Initialize VBO
		unsigned int size = width * height * sizeof(float) * 5;
		glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	inline void DeleteVBO(VertexBuffer* _vbo)
	{
		glDeleteBuffers(1, _vbo);
	}

	inline void BufferBind(const VertexBuffer &vbo){
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}

	inline void MapBuffer(CRAB::Vector4Df *color_buffer, int width, int height){
		//RayPathRender(triangles, cam, accumulatebuffer);

		float *buffer = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		CRAB::Vector4Df *c_buff = color_buffer;
		
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++, c_buff++) {
				buffer[(y*width + x) * 5] = (float) x;
				buffer[(y*width + x) * 5 + 1] = (float) y;
				memcpy(&buffer[(y*width + x) * 5 + 2], c_buff, sizeof(float) * 3);
			}
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glVertexPointer(2, GL_FLOAT, 20, 0);
		glColorPointer(4, GL_FLOAT, 20, (GLvoid*)8);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_POINTS, 0, width * height);
		glDisableClientState(GL_VERTEX_ARRAY);
	}


	inline void SwapBuffers(){
		glutSwapBuffers();
	}

	inline void KeyboardFunc(void(*callback)(unsigned char, int, int))		{ glutKeyboardFunc(callback); }
	inline void ReshapeFunc(void(*callback)(int, int))						{ glutReshapeFunc(callback); }
	inline void DisplayFunc(void(*callback)(void))							{ glutDisplayFunc(callback); }
	inline void MouseFunc(void(*callback)(int, int, int, int))				{ glutMouseFunc(callback); }
	inline void MotionFunc(void(*callback)(int, int))						{ glutMotionFunc(callback); }
	
}