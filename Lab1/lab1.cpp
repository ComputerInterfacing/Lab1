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
void StartTask2(void);
void StartTask3(void);
void Task1Main(void * pd);
void Task2Main(void * pd);
void Task3Main(void * pd);
}




/* Task stacks for all the user tasks */
/* If you add a new task you'll need to add a new stack for that task */
DWORD Task1Stk[USER_TASK_STK_SIZE] __attribute__( ( aligned( 4 ) ) );
DWORD Task2Stk[USER_TASK_STK_SIZE] __attribute__( ( aligned( 4 ) ) );
DWORD Task3Stk[USER_TASK_STK_SIZE] __attribute__( ( aligned( 4 ) ) );


const char * AppName="Abuni and Kathleen";


/* User task priorities always based on MAIN_PRIO */
/* The priorities between MAIN_PRIO and the IDLE_PRIO are available */
#define TASK1_PRIO 	MAIN_PRIO + 1
#define TASK2_PRIO 	MAIN_PRIO + 2
#define TASK3_PRIO	MAIN_PRIO + 3

#define WAIT_FOREVER 0


LCD myLCD;
const BYTE dollar[] = {0x00,0x24,0x2A, 0x7f, 0x2a, 0x12,0x00};
const BYTE sprite1[] = {0xAA, 0x55,0xAA, 0x55,0xAA, 0x55,0xAA}; //random sprite
OS_SEM Semaphore1;
OS_SEM Semaphore2;
OS_SEM Semaphore3;
OS_SEM Semaphore4;
OS_SEM Semaphore5;
void UserMain(void * pd) {
	BYTE err = OS_NO_ERR;
	//char * welcome = "I";

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
//    myLCD.DrawString(welcome);
//    myLCD.DrawString(welcome);
//    myLCD.DrawChar(dollar,char_index[9]);

    OSSemInit (&Semaphore1 , 0);
    OSSemInit (&Semaphore2 , 0);
    OSSemInit (&Semaphore3 , 0);
    OSSemInit (&Semaphore4 , 0);
//    OSSemInit (&Semaphore5 , 0);
//    OSSemPend(&Semaphore5,WAIT_FOREVER);//Start Again
    StartTask1();
    StartTask2();
    StartTask3();
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

/* Name: StartTask2
 * Description: Creates the task main loop.
 * Inputs: none
 * Outputs: none
 */
void StartTask2(void) {

	BYTE err = OS_NO_ERR;
	err = display_error( "StartTask2 fail:",
					OSTaskCreatewName(	Task2Main,
					(void *)NULL,
				 	(void *) &Task2Stk[USER_TASK_STK_SIZE],
				 	(void *) &Task2Stk[0],
				 	TASK2_PRIO, "Task 2" ));
}

/* Name: StartTask3
 * Description: Creates the task main loop.
 * Inputs: none
 * Outputs: none
 */
void StartTask3(void) {
	BYTE err = OS_NO_ERR;
	err = display_error( "StartTask3 fail:",
					OSTaskCreatewName(	Task3Main,
					(void *)NULL,
				 	(void *) &Task3Stk[USER_TASK_STK_SIZE],
				 	(void *) &Task3Stk[0],
				 	TASK3_PRIO, "Task 3" ));
}

/* Name: Task1Main
 * Description:
 * Inputs:  void * pd -- pointer to generic data . Currently unused.
 * Outputs: none
 */
void Task1Main( void * pd) {
	BYTE err = OS_NO_ERR;
	/* place semaphore usage code inside the loop */
//	OSSemPost(&Semaphore5);//Start Again
//    OSSemPend(&Semaphore5,WAIT_FOREVER);//Start Again
	while(1){
		for (int i = 0; i < LINE2_ORIGIN; i++){
				myLCD.DrawChar(sprite1,char_index[i]);
				OSTimeDly(TICKS_PER_SECOND);

			}
			myLCD.DrawChar(sprite1,char_index[LINE2_END]);
			OSSemPost(&Semaphore1);
			OSSemPend(&Semaphore4, WAIT_FOREVER);
			myLCD.DrawChar(sprite1,char_index[LINE2_ORIGIN]);
			OSTimeDly(TICKS_PER_SECOND);
			myLCD.Clear();
	}

}

void Task2Main( void * pd) {
	BYTE err = OS_NO_ERR;
	/* place semaphore usage code inside the loop */
	while(1){
		OSSemPend(&Semaphore1, WAIT_FOREVER);
			myLCD.DrawChar(sprite1,char_index[LINE3_END]);
			OSTimeDly(TICKS_PER_SECOND);
			myLCD.DrawChar(sprite1,char_index[LINE4_END]);
			OSTimeDly(TICKS_PER_SECOND);

			OSSemPost(&Semaphore2);
			OSSemPend(&Semaphore3, WAIT_FOREVER);
			myLCD.DrawChar(sprite1,char_index[LINE4_ORIGIN]);
			OSTimeDly(TICKS_PER_SECOND);
			myLCD.DrawChar(sprite1,char_index[LINE3_ORIGIN]);

			OSSemPost(&Semaphore4);
	}


}

void Task3Main( void * pd) {
	BYTE err = OS_NO_ERR;
	while(1){
		OSSemPend(&Semaphore2, WAIT_FOREVER);
			myLCD.DrawChar(sprite1,char_index[LINE5_END]);
			/* place semaphore usage code inside the loop */
			for (int i = LINE6_END; i >= LINE6_ORIGIN ; i--){
				myLCD.DrawChar(sprite1,char_index[i]);
				OSTimeDly(TICKS_PER_SECOND);
			}
			myLCD.DrawChar(sprite1,char_index[LINE5_ORIGIN]);
			OSSemPost(&Semaphore3);
	}


}
