
#include "includes.h"

extern int killTheGame;

extern OS_EVENT* newScoreMailBox; //send the total score to gui task messageBox

static int totalScore = 0;

#define MILIS_TO_WAIT_BETWEEN_EACH_LOOP 999

void scoreTask()
{
	
	INT8U err;

	while (!killTheGame)
	{
		
		totalScore = OSTimeGet() / 100;
		err = OSMboxPost(newScoreMailBox, &totalScore);
		
		if (err != OS_ERR_NONE && err != OS_ERR_MBOX_FULL)
		{
		
			sendMassage(MESSAGE_BOX_FULL_SCORE);
			killTheGame = 1;
			break;
		}

		OSTimeDlyHMSM(0, 0, 0, MILIS_TO_WAIT_BETWEEN_EACH_LOOP);
	}

	sendMassage(SCORE_FINISH);

}