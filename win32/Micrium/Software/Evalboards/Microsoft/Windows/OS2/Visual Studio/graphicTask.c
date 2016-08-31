#include "graphicTask.h"
#include <includes.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Mat3d.h"
#include "imageObject.h"
#include "ArrayList.h"
#include <stdlib.h>

#define FRAMES_PER_SEC	60
#define BIRD_SIZE		120
#define BIRD_COLLISION_SIZE (BIRD_SIZE / 8)
#define TOWER_WIDTH 100
#define TOWER_HEIGHT 350
#define MAX_NORMAL_VELOCITY 100
#define GRAVITY_FORCE 200.f
#define VELOCITY_PUSH 150

// Window dimensions
const int WIDTH = 800, HEIGHT = 600;

const int TIME_OF_FRAME = (1000 / FRAMES_PER_SEC);

float birdHeight = 0;
int isCollided = 0;
//extern int killTheGame;
//extern OS_EVENT* waitForWindowToInitilaize;


GLFWwindow* initWindow();
void clear(GLFWwindow* window);
void moveObject(void*);
void findCollision(void* o);
double x = 0;

void spacePressed(GLFWwindow* window, int, int, int, int); //remove after merge
int spacePressedFlag = 0;

void graphicTask(void *p_arg)
{
	GLFWwindow* window = initWindow();
	glfwSetKeyCallback(window, spacePressed);

	if (window == NULL)
		return;
	
	//GLFWwindow** windowPtr = (GLFWwindow**)p_arg;
	//*windowPtr = window;
	//OSSemPost(waitForWindowToInitilaize);
	while (!glfwWindowShouldClose(window))
	{
		x = 0;
		birdHeight = 0;
		initImageObjectShader();
		GLuint birdIde = initTexture("bird_Ide.png");
		GLuint birdUp = initTexture("bird_up.png");
		GLuint birdDown = initTexture("bird_down.png");
		GLuint tower = initTexture("tower.png");
		imageObject_t birdObj = initImageObject(birdIde, VEC3(0.0f, 0, -0.8f), BIRD_SIZE, BIRD_SIZE, 0, 0);

		ArrayList* arr = createArrayList();

		int xLastInt = (int)x;
		int collisions = 0;
		float velocityUp = 0;
		float forceGravety = GRAVITY_FORCE / FRAMES_PER_SEC;
		// Game Renderer loop
		while (1 /*&& !killTheGame*/)
		{
			OSTimeDlyHMSM(0, 0, 0, TIME_OF_FRAME);
			glfwPollEvents(); //remove after merge
			clear(window);

			velocityUp -= forceGravety;
			birdHeight += velocityUp / FRAMES_PER_SEC;
			if (spacePressedFlag)
			{
				velocityUp = VELOCITY_PUSH;
				birdObj.textureId = birdDown;
				spacePressedFlag = 0;
			}
			if (velocityUp < -MAX_NORMAL_VELOCITY)
			{
				birdObj.textureId = birdUp;
			}
			else if (velocityUp < MAX_NORMAL_VELOCITY)
			{
				birdObj.textureId = birdIde;
			}

			birdObj.position.Y_ = birdHeight;
			if (xLastInt != (int)x / 2)
			{
				xLastInt = (int)x / 2;
				int rnd = (rand(OSTimeGet()) % 200) - 100;
				imageObject_t* tower1 = (imageObject_t*)malloc(sizeof(imageObject_t));
				*tower1 = initImageObject(tower, VEC3((WIDTH + TOWER_WIDTH) / 2, HEIGHT / 2 - 75 + rnd, 0), TOWER_WIDTH, TOWER_HEIGHT, 1, x);
				pushItemToEndArrayList(arr, tower1);
				imageObject_t* tower2 = (imageObject_t*)malloc(sizeof(imageObject_t));
				*tower2 = initImageObject(tower, VEC3((WIDTH + TOWER_WIDTH) / 2, -HEIGHT / 2 + 75 + rnd, 0), TOWER_WIDTH, TOWER_HEIGHT, 0, x);
				pushItemToEndArrayList(arr, tower2);
			}
			doOperationOnArrayList(arr, moveObject);
			doOperationOnArrayList(arr, draw);
			imageObject_t* lastTower = getNextItemFromStartArrayList(arr);
			if (lastTower != NULL && lastTower->position.X_ < -(WIDTH + BIRD_SIZE) / 2)
			{
				popNextItemFromStartArrayList(arr);
				popNextItemFromStartArrayList(arr);
			}
			x += 1.0 / FRAMES_PER_SEC;
			draw(&birdObj);
			doOperationOnArrayList(arr, findCollision);

			if (isCollided || birdHeight < -HEIGHT / 2 - BIRD_SIZE)
			{
				APP_TRACE("%d.collision\n", collisions);
				isCollided = 0;
				collisions++;
				break;
			}
		}
		//killTheGame = 1;
		// Terminate GLFW, clearing any resources allocated by GLFW.
		//doOperationOnArrayList(arr, free);
		freeShaderImageObject();
		freeArrayList(arr);
		freeShaderImageObject();
	}
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

	// Setup some OpenGL options
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return window;
}

void clear(GLFWwindow* window)
{
	// Swap the screen buffers
	glfwSwapBuffers(window);
	glClearColor(140 / 256.f, 231 / 256.f, 238 / 256.f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void moveObject(void* o)
{
	imageObject_t* imageObj = (imageObject_t*)o;
	imageObj->position.X_ = (imageObj->startPos - x) * FRAMES_PER_SEC * 3 + WIDTH / 2 + 50;
}

void findCollision(void* o)
{
	imageObject_t* imageObj = (imageObject_t*)o;
	if (imageObj->position.X_ < BIRD_SIZE / 2 && imageObj->position.X_ > -BIRD_SIZE / 2)
	{
		if (!imageObj->op)
		{
			if (birdHeight - BIRD_COLLISION_SIZE < imageObj->position.Y_ + 175)
				isCollided = 1;
		}
		else
		{
			if (birdHeight + BIRD_COLLISION_SIZE > imageObj->position.Y_ - 175)
				isCollided = 1;
		}
	}
}

void spacePressed(GLFWwindow * window, int key, int scanCode, int action, int mods) // remove when marge
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		spacePressedFlag = 1;
}
