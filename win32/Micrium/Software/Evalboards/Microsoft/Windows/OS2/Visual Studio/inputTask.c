

#include "includes.h"


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);



OS_EVENT* waitForWindowToInitilaizeSemaphore; // init in main value of 0
OS_EVENT* sendIfSpacePressedMailBox; // Mail box send int 1 if key As Been Clicked

extern int killTheGame;


#define NUMBER_OF_MILIS_PERETERATION 50 // 20 times in secound

void inputTask(void** rawWindow)
{
	GLFWwindow* window = (GLFWwindow*)rawWindow;
	INT8U getWindowError;

	OSSemPend(waitForWindowToInitilaizeSemaphore, 0, &getWindowError); // need to delete the sem after using -> on time sem

	if (getWindowError == OS_ERR_NONE)
	{

		glfwSetKeyCallback(window, key_callback);

		while (!killTheGame)
		{
			glfwPollEvents(); // call the key_callback function
			OSTimeDlyHMSM(0, 0, 0, NUMBER_OF_MILIS_PERETERATION );
		}

		printf("input task as been closed!\n");

	}
	else
	{
		printf("couldnt get the window! - in input task error number %d \n" , getWindowError);
		
		OSSchedLock();
		killTheGame = 1;
		OSSchedUnlock(); 
	}

}


static int isPressed = 0;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	INT8U error;
	if (key == GLFW_KEY_SPACE)
	{
		if (action == GLFW_PRESS)
		{
			if (!isPressed)
			{
				isPressed = 1;
				if ((error = OSMboxPost(sendIfSpacePressedMailBox, &isPressed)) == OS_ERR_MBOX_FULL)
				{
					printf("mail box is full . cannot send that key is pressed \n"); // nothing too bad 
				}
				else if (error == OS_ERR_PEVENT_NULL)
				{
					printf("message box didnt initialize in input task\n"); // something went awfull wrong
				}
			}
		}
		else if (action == GLFW_RELEASE)
		{
			isPressed = 0;
		}
	}
}