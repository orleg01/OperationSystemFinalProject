
#include "includes.h"

extern int killTheGame;

extern OS_EVENT* newScoreMailBox; //send the total score to gui task messageBox

static int totalScore = 0;

#define MILIS_TO_WAIT_BETWEEN_EACH_LOOP 1000

void scoreTask()
{
	
	INT8U err;

	while (!killTheGame)
	{
		
		totalScore = OSTimeGet() / 100;
		err = OSMboxPost(newScoreMailBox, &totalScore);
		
		if (err != OS_ERR_NONE)
		{
		
			printf("critic problem in score task with the message box");
			killTheGame = 1;
			break;
		}

		OSTimeDlyHMSM(0, 0, 0, MILIS_TO_WAIT_BETWEEN_EACH_LOOP);
	}

	printf("score task is finished");

}