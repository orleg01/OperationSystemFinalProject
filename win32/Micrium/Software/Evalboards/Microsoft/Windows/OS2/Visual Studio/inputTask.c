

#include "includes.h"


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);



extern OS_EVENT* waitForWindowToInitilaizeSemaphore; // init in main value of 0
extern OS_EVENT* sendIfSpacePressedMailBox; // Mail box send int 1 if key As Been Clicked

extern int killTheGame;

extern const int TIME_OF_FRAME;


//#define NUMBER_OF_MILIS_PERETERATION 50 // 20 times in secound

void inputTask(void* rawWindow)
{
	GLFWwindow* window = *(GLFWwindow**)rawWindow;
	INT8U getWindowError;

	OSSemPend(waitForWindowToInitilaizeSemaphore, 0, &getWindowError); // need to delete the sem after using -> on time sem
	//printf("%p\n", window);
	if (getWindowError == OS_ERR_NONE)
	{

		glfwSetKeyCallback(window, key_callback);

		/*while (!killTheGame)
		{
			//glfwPollEvents(); // call the key_callback function
			OSTimeDlyHMSM(0, 0, 0, TIME_OF_FRAME);
		}*/

		//printf("input task as been closed!\n");

	}
	else
	{
		OSSchedLock();
		killTheGame = 1;
		OSSchedUnlock(); 
	}

}




static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	INT8U error;
	if (key == GLFW_KEY_SPACE)
	{
		if (action == GLFW_PRESS)
		{
			if ((error = OSMboxPost(sendIfSpacePressedMailBox, (void*)1)) == OS_ERR_MBOX_FULL)
			{
				sendMassage(MAIL_BOX_IS_FULL);
			}
			else if (error == OS_ERR_PEVENT_NULL)
			{
				sendMassage(MESSAGE_BOX_DID_NOT_INIT);
			}
		}
	}
}