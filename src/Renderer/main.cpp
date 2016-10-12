#include <cstdio>
#include <cstring>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "RawModelAssimp.h"
#include "Renderer.h"

void glfw_error_callback(int error, const char* description)
{
	std::cerr << __FUNCTION__ << ": ";
	std::cerr << description << std::endl;
}

void APIENTRY OpenGLCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void * userParam)
{
	using namespace std;
	// Ignore certain messages
	switch (id)
	{
		// A verbose message about what type of memory was allocated for a buffer.
	case 131185:
		return;
	}

	cout << "---------------------opengl-callback-start------------" << endl;
	cout << "message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << endl;

	cout << "id: " << id << endl;
	cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	}
	cout << endl;
	cout << "---------------------opengl-callback-end--------------" << endl;
}

int main()
{
	glfwSetErrorCallback(glfw_error_callback);
	glfwInit();

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // DEBUG		

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Resource Man demo", nullptr, nullptr);

	if (!window) { glfwTerminate(); return -1; }

	glfwMakeContextCurrent(window);
	//glfwSetKeyCallback(window, glfwKeyCallback);

	glewInit();

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	if (glDebugMessageCallback) {
		glDebugMessageCallback(OpenGLCallbackFunction, nullptr);
	} else {
		std::cerr << "glDebugMessageCallback not available" << std::endl;
	}
#endif // DEBUG

	Renderer render;
	// Try to load a model and render it.
	
	auto teapotHandle = ResourceManager::Load<RawModelAssimp>("C:/Users/Dennis Olsen/Documents/GitHub/Spelmotorarkitekturer/teapot.obj", 0);
	auto teapotResource = *teapotHandle;

	//RawModelAssimp model("C:/Users/Dennis Olsen/Documents/GitHub/Spelmotorarkitekturer/teapot.obj");

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//render.Render(teapotResource);
		
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glEnd();

		glfwSwapBuffers(window);
	}

	return 0;
}