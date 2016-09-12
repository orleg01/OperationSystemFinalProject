#ifndef __MASSAGE_TASK__H
#define __MASSAGE_TASK__H

enum {
	//main
	INIT_PROBLEM,

	//task input
	MAIL_BOX_IS_FULL,
	MESSAGE_BOX_DID_NOT_INIT,

	//task mapMoving
	MAP_CRITIC_PROBLEM ,
	MAP_NEW_DELTA_TIME ,
	MAP_FINISH ,

	//task graphics
	FAIL_TO_CREATE_GLFW_WINDOW,
	FAILED_TO_INIT_GLFW,
	SOME_ARGUMENT_IS_NULL,
	ERROR_SHADER_VERTEX_COMPILATION_FAILED,
	ERROR_SHADER_FRAGMENT_COMPILATION_FAILED,
	ERROR_SHADER_PROGRAM_LINKING_FAILED,
	//task score
	
	MESSAGE_BOX_FULL_SCORE,
	SCORE_FINISH ,
	//task screen color
};


void massageTask(void* p_arg);
void sendMassage(int message);
#endif