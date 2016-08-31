/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*                          (c) Copyright 2009-2011; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                          APPLICATION CODE
*
*                                          Microsoft Windows
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : JJL
*                 FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];
static  CPU_STK  GraphicTasckStk[APP_TASK_START_STK_SIZE];
static  CPU_STK  KeyBoardTasckStk[APP_TASK_START_STK_SIZE];
static  CPU_STK  MapMovingTasckStk[APP_TASK_START_STK_SIZE];
static  CPU_STK  ScoreTasckStk[APP_TASK_START_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart(void  *p_arg);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

#include "Visual Studio\ArrayList.h"

int killTheGame = 0;

OS_EVENT* newScoreMailBox;
OS_EVENT* whereTheCameraStandsMailBox;
OS_EVENT* waitForWindowToInitilaizeSemaphore;
OS_EVENT* sendIfSpacePressedMailBox;

ArrayList* listOfAllTaskTime; // real item in the context switch hook

static void OsEventInit()
{

	listOfAllTaskTime = createArrayList(); // add task when create in create task hook 


	if ((newScoreMailBox = OSMboxCreate(newScoreMailBox)) != (OS_EVENT *)0)
		if ((whereTheCameraStandsMailBox = OSMboxCreate(whereTheCameraStandsMailBox)) != (OS_EVENT *)0)
			if ((sendIfSpacePressedMailBox = OSMboxCreate(sendIfSpacePressedMailBox)) != (OS_EVENT *)0)
				if ((waitForWindowToInitilaizeSemaphore = OSSemCreate(0)) != (OS_EVENT *)0)
					return;

	printf("cannot initialize the project!!!\n");
	killTheGame = 1;

}


int  main (void)
{
    OSInit();                                                   /* Init uC/OS-II.                                       */

	OsEventInit();

    OSTaskCreateExt((void(*)(void *))AppTaskStart,              /* Create the start task                                */
        (void          *) 0,
        (OS_STK        *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
        (INT8U          ) APP_TASK_START_PRIO,
        (INT16U         ) APP_TASK_START_PRIO,
        (OS_STK        *)&AppTaskStartStk[0],
        (INT32U         ) APP_TASK_START_STK_SIZE,
        (void          *) 0,
        (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

	OSTaskCreateExt((void(*)(void *))graphicTask,              /* Create the start task                                */
		(void          *) 0,
		(OS_STK        *)&GraphicTaskStk[APP_TASK_START_STK_SIZE - 1],
		(INT8U			) GRAPHIC_TASK_PRIO,
		(INT16U			) GRAPHIC_TASK_PRIO,
		(OS_STK        *)&GraphicTaskStk[0],
		(INT32U			) APP_TASK_START_STK_SIZE,
		(void          *) 0,
		(INT16U			)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

	OSTaskCreateExt((void(*)(void **))inputTask,              /* Create the start task                                */
		(void          *)0,
		(OS_STK        *)&KeyBoardTasckStk[APP_TASK_START_STK_SIZE - 1],
		(INT8U)KEYBOARD_TASK_PRIO,
		(INT16U)KEYBOARD_TASK_PRIO,
		(OS_STK        *)&KeyBoardTasckStk[0],
		(INT32U)APP_TASK_START_STK_SIZE,
		(void          *)0,
		(INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

	OSTaskCreateExt((void(*)(void *))mapMovingTask,              /* Create the start task                                */
		(void          *)0,
		(OS_STK        *)&MapMovingTasckStk[APP_TASK_START_STK_SIZE - 1],
		(INT8U)MAP_MOVE_TASK_PRIO,
		(INT16U)MAP_MOVE_TASK_PRIO,
		(OS_STK        *)&MapMovingTasckStk[0],
		(INT32U)APP_TASK_START_STK_SIZE,
		(void          *)0,
		(INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

	OSTaskCreateExt((void(*)(void *))scoreTask,              /* Create the start task                                */
		(void          *)0,
		(OS_STK        *)&ScoreTasckStk[APP_TASK_START_STK_SIZE - 1],
		(INT8U)SCORE_TASK_PRIO,
		(INT16U)SCORE_TASK_PRIO,
		(OS_STK        *)&ScoreTasckStk[0],
		(INT32U)APP_TASK_START_STK_SIZE,
		(void          *)0,
		(INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Argument(s) : p_arg       is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Note(s)     : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    //OS_ERR  err;

   (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();                                                 /* Initialize uC/CPU services                           */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif
}
