#include "graphicTask.h"
#include <includes.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Mat3d.h"

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
const int FRAMES_PER_SECOND = 60;

GLFWwindow* initWindow();

void graphicTask(void *p_arg)
{
	GLFWwindow* window = initWindow();
	if (window == NULL)
		return;

	// Game Renderer loop
	while (!glfwWindowShouldClose(window))
	{
		// Swap the screen buffers
		glfwSwapBuffers(window);
		OSTimeDlyHMSM(0, 0, 0, 1000 / FRAMES_PER_SECOND);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

GLFWwindow* initWindow()
{
	APP_TRACE("Starting GLFW context, OpenGL 3.3\n");
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		APP_TRACE("Failed to create GLFW window\n");
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		APP_TRACE("Failed to initialize GLEW\n");
		return NULL;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	return window;
}