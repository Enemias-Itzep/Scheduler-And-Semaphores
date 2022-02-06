/*
 * G8RTOS_Scheduler.c
 */

/*********************************************** Dependencies and Externs *************************************************************/

#include <stdint.h>
#include <driverlib.h>
#include <G8RTOS/G8RTOS_Scheduler.h>
#include <G8RTOS/G8RTOS_Structures.h>
#include "msp.h"
#include "BSP.h"

/*
 * G8RTOS_Start exists in asm
 */
extern void G8RTOS_Start();

/* System Core Clock From system_msp432p401r.c */
extern uint32_t SystemCoreClock;

/*
 * Pointer to the currently running Thread Control Block
 */
extern tcb_t * CurrentlyRunningThread;

/*********************************************** Dependencies and Externs *************************************************************/


/*********************************************** Defines ******************************************************************************/

/* Status Register with the Thumb-bit Set */
#define THUMBBIT 0x01000000

/*********************************************** Defines ******************************************************************************/


/*********************************************** Data Structures Used *****************************************************************/

/* Thread Control Blocks
 *	- An array of thread control blocks to hold pertinent information for each thread
 */
static tcb_t threadControlBlocks[MAX_THREADS];

/* Thread Stacks
 *	- An array of arrays that will act as individual stacks for each thread
 */
static int32_t threadStacks[MAX_THREADS][STACKSIZE]; //6 1024


/*********************************************** Data Structures Used *****************************************************************/


/*********************************************** Private Variables ********************************************************************/

/*
 * Current Number of Threads currently in the scheduler
 */
static uint32_t NumberOfThreads;

/*********************************************** Private Variables ********************************************************************/


/*********************************************** Private Functions ********************************************************************/

/*
 * Initializes the Systick and Systick Interrupt
 * The Systick interrupt will be responsible for starting a context switch between threads
 * Param "numCycles": Number of cycles for each systick interrupt
 */
static void InitSysTick(uint32_t numCycles)
{
    SysTick_Config(numCycles); //Configures SysTick overflow by amount of cycles
}

/*
 * Chooses the next thread to run.
 * Lab 2 Scheduling Algorithm:
 * 	- Simple Round Robin: Choose the next running thread by selecting the currently running thread's next pointer
 */
void G8RTOS_Scheduler()
{
	//Sets the current thread to the next thread
    CurrentlyRunningThread = (*CurrentlyRunningThread).next;
}

/*
 * SysTick Handler
 * Currently the Systick Handler will only increment the system time
 * and set the PendSV flag to start the scheduler
 *
 * In the future, this function will also be responsible for sleeping threads and periodic threads
 */
void SysTick_Handler()
{
	//Increments system time
    SystemTime++;

    //Sets PendSV flag
    SCB->ICSR |= (1<<28);
}

/*********************************************** Private Functions ********************************************************************/

/*********************************************** Public Variables *********************************************************************/

/* Holds the current time for the whole System */
uint32_t SystemTime;

/*********************************************** Public Variables *********************************************************************/


/*********************************************** Public Functions *********************************************************************/

/*
 * Sets variables to an initial state (system time and number of threads)
 * Enables board for highest speed clock and disables watchdog
 */
void G8RTOS_Init()
{
    //Sets system time to 0
    SystemTime = 0;

    //Sets number of threads to 0
    NumberOfThreads = 0;

    //Initializes board
    BSP_InitBoard();
}

/*
 * Starts G8RTOS Scheduler
 * 	- Initializes the Systick
 * 	- Sets Context to first thread
 * Returns: Error Code for starting scheduler. This will only return if the scheduler fails
 */
int G8RTOS_Launch()
{
    //Sets currently running thread to first tcb
	CurrentlyRunningThread = &threadControlBlocks[0];

	//Gets clock frequency
	uint32_t clkFreq = ClockSys_GetSysFreq();

	//Initializes SysTick
	InitSysTick((clkFreq/1000));

	//Sets priorities for PENDSV and SysTick
	NVIC_SetPriority(PendSV_IRQn, 7);

	//Call G8RTOS_Start
	G8RTOS_Start();

	return ERROR;
}


/*
 * Adds threads to G8RTOS Scheduler
 * 	- Checks if there are still available threads to insert to scheduler
 * 	- Initializes the thread control block for the provided thread
 * 	- Initializes the stack for the provided thread to hold a "fake context"
 * 	- Sets stack tcb stack pointer to top of thread stack
 * 	- Sets up the next and previous tcb pointers in a round robin fashion
 * Param "threadToAdd": Void-Void Function to add as preemptable main thread
 * Returns: Error code for adding threads
 */
int G8RTOS_AddThread(void (*threadToAdd)(void)) { //Funtions address

    //If maximum threads have not been reached
    if(MAX_THREADS != NumberOfThreads){
        //Creates new struct and initializes tcb to thread
        struct tcb_t thread;

        //When no threads exist
        if (NumberOfThreads == 0){
            //Points new thread to self
            thread.next = &thread;
            thread.prev = &thread;
        }
        //When final thread is added
        else if (NumberOfThreads == 5){
            thread.prev = &threadControlBlocks[NumberOfThreads - 1];
            thread.next = &threadControlBlocks[0];

            threadControlBlocks[NumberOfThreads - 1].next = &threadControlBlocks[NumberOfThreads];
            threadControlBlocks[0].prev = &threadControlBlocks[NumberOfThreads];
        }
        else{

            thread.prev = &threadControlBlocks[NumberOfThreads - 1];
            thread.next = &threadControlBlocks[0];

            threadControlBlocks[NumberOfThreads - 1].next = &threadControlBlocks[NumberOfThreads];
            threadControlBlocks[0].prev = &threadControlBlocks[NumberOfThreads];
        }

        //Sets thumbbit in xPSR
        threadStacks[NumberOfThreads][STACKSIZE - 1] = THUMBBIT;

        //Sets PC to function pointer
        threadStacks[NumberOfThreads][STACKSIZE - 2] = (int32_t)threadToAdd;

        //Fills R0 to R14 with dummy values
        int i;
        for (i = 3; i <= 16; i++)
            threadStacks[NumberOfThreads][STACKSIZE - i] = 1;

        //Sets sp pointer to point to top of stack pointer address
        thread.sp = &threadStacks[NumberOfThreads][STACKSIZE - 16];

        //Adds thread
        threadControlBlocks[NumberOfThreads] = thread;

        //Increments number of threads
        NumberOfThreads++;
    }
    else{
        return ERROR;
    }
}

/*********************************************** Public Functions *********************************************************************/
