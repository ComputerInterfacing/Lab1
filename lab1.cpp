#include <predef.h>
#include <stdio.h>
#include <ctype.h>
#include <startnet.h>
#include <autoupdate.h>
#include <smarttrap.h>
#include <taskmon.h>
#include <NetworkDebug.h>
#include <pinconstant.h>
#include <pins.h>
#include <basictypes.h>
#include "LCD.h"
#include "bitmaps.h"
#include "error_wrapper.h"
#include "point.h"


extern "C" {
void UserMain(void * pd);
void StartTask1(void);
void Task1Main(void * pd);
}




/* Task stacks for all the user tasks */
/* If you add a new task you'll need to add a new stack for that task */
DWORD Task1Stk[USER_TASK_STK_SIZE] __attribute__( ( aligned( 4 ) ) );


const char * AppName="Put your name here";


/* User task priorities always based on MAIN_PRIO */
/* The priorities between MAIN_PRIO and the IDLE_PRIO are available */
#define TASK1_PRIO 	MAIN_PRIO + 1
#define TASK2_PRIO 	MAIN_PRIO + 2
#define TASK3_PRIO	MAIN_PRIO + 3

#define WAIT_FOREVER 0


LCD myLCD;

void UserMain(void * pd) {
	BYTE err = OS_NO_ERR;
	char * welcome = "Welcome";

    InitializeStack();
    OSChangePrio(MAIN_PRIO);
    EnableAutoUpdate();
    StartHTTP();
    EnableTaskMonitor();

    #ifndef _DEBUG
    EnableSmartTraps();
    #endif

    #ifdef _DEBUG
    InitializeNetworkGDB_and_Wait();
    #endif

    iprintf("Application started: %s\n",AppName );

    myLCD.Init();
    myLCD.Clear();
    myLCD.Home();
    myLCD.DrawString(welcome);

    StartTask1();

    while (1) {
        OSTimeDly(TICKS_PER_SECOND);
    }
}


/* Name: StartTask1
 * Description: Creates the task main loop.
 * Inputs: none
 * Outputs: none
 */
void StartTask1(void) {
	BYTE err = OS_NO_ERR;

	err = display_error( "StartTask1 fail:",
					OSTaskCreatewName(	Task1Main,
					(void *)NULL,
				 	(void *) &Task1Stk[USER_TASK_STK_SIZE],
				 	(void *) &Task1Stk[0],
				 	TASK1_PRIO, "Task 1" ));
}

/* Name: Task1Main
 * Description:
 * Inputs:  void * pd -- pointer to generic data . Currently unused.
 * Outputs: none
 */
void	Task1Main( void * pd) {


	BYTE err = OS_NO_ERR;

	/* place semaphore usage code inside the loop */
	while (1) {
		OSTimeDly(TICKS_PER_SECOND);
	}
}
