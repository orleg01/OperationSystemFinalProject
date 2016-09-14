
#include "includes.h"

extern int killTheGame;
extern OS_EVENT* sendNewBackGroundColoeMailBox;

static Color color;

#define NUMBER_OF_SEC_BETWEEN_BACKGROUND_CHANGE 6



void colorChangeTask()
{
	srand((unsigned)time(NULL));

	while (!killTheGame)
	{
		
		color.red = rand()%127 + 127;
		color.green = rand() % 127 + 127;
		color.blue = rand() % 127 + 127;

		OSMboxPost(sendNewBackGroundColoeMailBox, &color);

		OSTimeDlyHMSM(0, 0, NUMBER_OF_SEC_BETWEEN_BACKGROUND_CHANGE, 0);
	}
}