

#include "includes.h"


extern const int TIME_OF_FRAME; //1000 milisec : fps
extern int killTheGame;

extern OS_EVENT* whereTheCameraStandsMailBox; // send the gui thread where its camera have to be stands -> send float 

static double cameraXStandPoint = 0;
static double xDeltaTime = 0.1;
static double wantedXDeltaTime = 0.1;



#define X_DELTA_MIN 0.05
#define X_DELTA_MAX (double)0.4
#define NUMBER_OF_TICK_BETWEEN_EACH_VALOCITY_AIM_CHANGE  50 // check if needed to change while play

static void printHelpfullMessage(INT8U err)
{
	switch (err)
	{
	case OS_ERR_MBOX_FULL:
		printf("map moving task -> message box isnt empty cannot send the new variable at");
		return;
	default:
		printf("critic problem at map moving task");
		OSSchedLock();
		killTheGame = 1;
		OSSchedUnlock();
	}
}

static void getNewDeltaTime() 
{
	
	printf("get new delta time aim at map moving task\n");

	double tempNum = ((double)rand()/(double)RAND_MAX*(X_DELTA_MAX - X_DELTA_MIN)) + X_DELTA_MIN;

	OSSchedLock();

	wantedXDeltaTime = tempNum;

	OSSchedUnlock();
}

void mapMovingTask()
{

	time_t t;
	const double deltaTime = TIME_OF_FRAME * 0.001;
	INT8U error;

	srand((unsigned)time(&t));

	while (!killTheGame)
	{
		if (OSTimeGet() % NUMBER_OF_TICK_BETWEEN_EACH_VALOCITY_AIM_CHANGE == 0)
			getNewDeltaTime();

		xDeltaTime += (wantedXDeltaTime - xDeltaTime)*deltaTime;
		cameraXStandPoint += xDeltaTime;


		error = OSMboxPost(whereTheCameraStandsMailBox, &cameraXStandPoint);

		if (error != OS_ERR_NONE)
			printHelpfullMessage(error);


		OSTimeDlyHMSM(0, 0, 0, TIME_OF_FRAME);
	}

	printf("map moving task as been closed!\n");


}