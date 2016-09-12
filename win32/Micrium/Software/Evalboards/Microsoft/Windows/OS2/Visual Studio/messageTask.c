#include "messageTask.h"

#include "includes.h"

OS_EVENT* errorQ;

extern const int numberOfTasks;
extern int killTheGame;
int* messageQueStorage[6];

void printLogger(int num)
{
	char* temp;
	switch (num)
	{
	case INIT_PROBLEM:
		temp = "problem with init variables";
		break;
	case MESSAGE_BOX_FULL_SCORE:
		temp = "critic problem in score task with the message box";
		break;
	case SCORE_FINISH :
		temp = "score task is finished";
		break;
	case MAP_CRITIC_PROBLEM:
		temp = "critic problem at map moving task\n";
		break;
	case MAP_NEW_DELTA_TIME :
		temp = "get new delta time aim at map moving task\n";
		break;

	case MAP_FINISH :
		temp = "map moving task as been closed!\n";
		break;

	case MAIL_BOX_IS_FULL:
		temp = "mail box is full . cannot send that key is pressed \n"; // nothing too bad
		break;

	case MESSAGE_BOX_DID_NOT_INIT:
		temp = "message box didnt initialize in input task\n"; // something went awfull wrong
		break;

	case FAIL_TO_CREATE_GLFW_WINDOW:
		temp = "Failed to create GLFW window\n";
		break;

	case FAILED_TO_INIT_GLFW:
		temp = "Failed to initialize GLEW\n";
		break;

	case SOME_ARGUMENT_IS_NULL:
		temp = "some argument is null check your variables.";
		break;

	case ERROR_SHADER_VERTEX_COMPILATION_FAILED:
		temp = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
		break;

	case ERROR_SHADER_FRAGMENT_COMPILATION_FAILED:
		temp = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
		break;

	case ERROR_SHADER_PROGRAM_LINKING_FAILED:
		temp = "ERROR::SHADER::PROGRAM::LINKING_FAILED\n";
		break;
	}

	printf(temp);
}

void errorHandler(INT8U err)
{
	switch (err)
	{
	
	case OS_ERR_NONE:
		return;
	case OS_ERR_TIMEOUT:
		APP_TRACE("A message was not received within the specified 'timeout'. In task [%3d]\n" , OSTCBCur->OSTCBId);
		killTheGame = 1;
		return;
	case OS_ERR_EVENT_TYPE:
		APP_TRACE("The wait on the queue was aborted. In task [%3d]\n", OSTCBCur->OSTCBId);
		killTheGame = 1;
		return;
	case OS_ERR_PEVENT_NULL:
		APP_TRACE("If 'pevent' is a NULL pointer. In task [%3d]\n", OSTCBCur->OSTCBId);
		killTheGame = 1;
		return;
	case OS_ERR_PEND_LOCKED:
		APP_TRACE("If you called this function with the scheduler is locked. In task [%3d]\n", OSTCBCur->OSTCBId);
		killTheGame = 1;
		return;
	case OS_ERR_PEND_ISR:
		APP_TRACE("If you called this function from an ISR and the result. In task [%3d]\n", OSTCBCur->OSTCBId);
		killTheGame = 1;
		return;
	}

}

void massageTask(void* p_arg)
{
	INT8U err;

	int printEndMessages = 0;

	while (!killTheGame || ! printEndMessages)
	{

		int pendNum = (int)OSQPend(errorQ, 0, &err);

		printEndMessages = killTheGame;
		errorHandler(err);

		printLogger(pendNum);

	}
}

void sendMassage(int message)
{
	OSQPost(errorQ, message);
}