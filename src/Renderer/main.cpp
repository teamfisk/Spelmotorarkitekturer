#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "RawModelAssimp.h"
#include "Renderer.h"
#include "Camera.h"
#include "Shader.h"
#include "zlib.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "ResourceManager/STUFF/STUFFBundle.h"
#include "ResourceManager/FilesystemBundle.h"
#include "CameraEndlessRunner.h"
#include "Entity.h"

using namespace std;

const int ENTITY_MOVE_DIST_X = 50;

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
    ResourceManager::RegisterBundleFormat<STUFFBundle>();
	ResourceManager::RegisterBundleFormat<FilesystemBundle>();
	ResourceManager::RegisterBundle("Resources");
	ResourceManager::Initialize();

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

	//glEnable(GL_DEPTH_BUFFER); // This is apparently not a thing, but Intel does not warn about it. Nvidia does though.
	glEnable(GL_DEPTH_TEST);

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	if (glDebugMessageCallback) {
		glDebugMessageCallback(OpenGLCallbackFunction, nullptr);
	} else {
		std::cerr << "glDebugMessageCallback not available" << std::endl;
	}

	int major, min, rev;
	glfwGetVersion(&major, &min, &rev);
	cout << "GLFW version: " << major << "." << min << "." << rev << endl;
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

#endif // DEBUG
	// Set the camera just above the plane, pointing in the direction of the x-axis.
	CameraEndlessRunner camera;
	camera.setPosition({ -20.94543, 0.807405,  0.810452 });
	camera.SetDirection({ 1, 0, 0 });

	vector<ShaderInfo> shader;
	loadShader("Resources/vertex.glsl", GL_VERTEX_SHADER, shader);
	loadShader("Resources/fragment.glsl", GL_FRAGMENT_SHADER, shader);
	GLuint programHandle;
	compileShaderProgram(shader, programHandle);

	Renderer renderer;

	// Try to load a model and render it.	
	auto teapotHandle = ResourceManager::Load<Model>("teapot.obj");
	auto planeHandle = ResourceManager::Load<Model>("plane.obj");
	
	auto catTexHandle = ResourceManager::Load<Texture>("kitten_png_image.png");

	
	//auto bunnyHandle = ResourceManager::Load<Model>("bunny.obj", 0);

	// Load other models when needed during runtime, to minimize initial load time.

	glm::mat4 worldMat = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
		
	auto keyILastFrame = GLFW_RELEASE;

	glClearColor(0.f, 0.0f, 0.3f, 1.f);
	double lastTime = glfwGetTime();

	std::vector<Entity> entities;
	entities.emplace_back(translate(glm::vec3(0, 0, 0)), teapotHandle, catTexHandle);
	entities.emplace_back(translate(glm::vec3(5, 0, 3)), teapotHandle, catTexHandle);
	entities.emplace_back(translate(glm::vec3(9, 0, 0)), teapotHandle, catTexHandle);
	entities.emplace_back(translate(glm::vec3(16, 0, 4)), teapotHandle, catTexHandle);
	entities.emplace_back(translate(glm::vec3(22, 0, -2)), teapotHandle, catTexHandle);
	entities.emplace_back(translate(glm::vec3(27, 0, 1)), teapotHandle, catTexHandle);
	entities.emplace_back(translate(glm::vec3(32, 0, 4)), teapotHandle, catTexHandle);
	entities.emplace_back(translate(glm::vec3(40, 0, -2)), teapotHandle, catTexHandle);
	entities.emplace_back(translate(glm::vec3(45, 0, 1)), teapotHandle, catTexHandle);
	
	// Planes
	glm::mat4x4 planeMatrix = glm::rotate((float)M_PI_2, glm::vec3(1.0, 0.0, 0.0));
	planeMatrix = glm::scale(planeMatrix, { 1, 1, 1 });

	entities.emplace_back(translate(glm::vec3(-20, 0, 0)) * planeMatrix, planeHandle, catTexHandle);
	entities.emplace_back(translate(glm::vec3(-10, 0, 0)) * planeMatrix, planeHandle, catTexHandle);
	entities.emplace_back(translate(glm::vec3(0, 0, 0)) * planeMatrix, planeHandle, catTexHandle);
	for (int i = 0; i < 6; i++)
	{
		entities.emplace_back(translate(glm::vec3(10 * i, 0, 0)) * planeMatrix, planeHandle, catTexHandle);
	}		

	double timeSinceLastEntitySpawn = 0;

	while (!glfwWindowShouldClose(window))
	{	
		double time = glfwGetTime();
		double dt = time - lastTime;
		lastTime = time;

		ResourceManager::ProcessAsyncQueue();
		//ResourceManager::Collect();

		for(unsigned int i = 0; i < entities.size(); i++)
		{
			float x = entities[i].worldMatrix[3][0];
			float offset = 6; 
			if (x + offset < camera.getPosition().x) // Trigger this slightly after the player has passed the entity.
			{				
				entities[i].worldMatrix[3][0] += ENTITY_MOVE_DIST_X;

				// Change models after some time. It's like a new level!!
				if (glfwGetTime() > 10.0f)
				{					
					entities[i].textureHandle = ResourceManager::Load<Texture>("moss 5.png");
				}
			}			
		}
		
		//timeSinceLastEntitySpawn += dt;
		//if (timeSinceLastEntitySpawn > 5.0)
		//{
		//	timeSinceLastEntitySpawn = 0;

		//	auto mat = glm::mat4x4(1);
		//	mat = translate(mat, camera.getPosition() + glm::vec3(25 + rand() % 5, -1, 0));
		//	entities.emplace_back(mat, teapotHandle);			
		//}		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Update();		
		
		auto keyICurrentFrame = glfwGetKey(window, GLFW_KEY_I);
		if(keyICurrentFrame == GLFW_PRESS && keyILastFrame == GLFW_RELEASE)
		{			
			keyILastFrame = GLFW_PRESS;
			worldMat = translate(worldMat, { 5, 0, 0 });
		}
		keyILastFrame = keyICurrentFrame;

		glUseProgram(programHandle);		
		glUniformMatrix4fv(glGetUniformLocation(programHandle, "projection"), 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(programHandle, "view"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));	
		
		for (unsigned int i = 0; i < entities.size(); i++)
		{
			auto& entity = entities[i];
			
			if (entity.modelHandle.Valid() && entity.textureHandle.Valid()) // Check that the handle is valid.
			{
				if (entity.modelHandle->Valid() && entity.textureHandle->Valid()) // Check that the resource the handle refers to is valid.
				{
					renderer.Render(*entity.modelHandle, entity.worldMatrix, *entity.textureHandle, programHandle);
				}
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}