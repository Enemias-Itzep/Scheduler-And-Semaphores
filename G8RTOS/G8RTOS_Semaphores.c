/*
 * G8RTOS_Semaphores.c
 */

/*********************************************** Dependencies and Externs *************************************************************/

#include <G8RTOS/G8RTOS_CriticalSection.h>
#include <G8RTOS/G8RTOS_Semaphores.h>
#include <stdint.h>
#include "msp.h"
#include "Threads.h"

/*********************************************** Dependencies and Externs *************************************************************/
semaphore_t LED;
semaphore_t Sensor;

/*********************************************** Public Functions *********************************************************************/

/*
 * Initializes a semaphore to a given value
 * Param "s": Pointer to semaphore
 * Param "value": Value to initialize semaphore to
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_InitSemaphore(semaphore_t *s, int32_t value)
{
    //Disables interrupts
    int32_t priMask = StartCriticalSection();

    //Initialize semaphore
    *s = value;

    //Enable interrupts
    EndCriticalSection(priMask);
}

/*
 * Waits for a semaphore to be available (value greater than 0)
 *  - Decrements semaphore when available
 *  - Spinlocks to wait for semaphore
 * Param "s": Pointer to semaphore to wait on
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_WaitSemaphore(semaphore_t *s)
{
    //Disable Interrupts
    int32_t priMask = StartCriticalSection();

    while((*s)== 0)
    {
        //Enable interrupts
        EndCriticalSection(priMask);

        //Added so that time is given to scheduler to change task
        uint32_t i;
        for (i = 0; i < 100; i++);

        //Disable interrupts
        priMask = StartCriticalSection();
    }
    //Decrement Semaphore
    (*s)--;

    //Enable Interrupts
    EndCriticalSection(priMask);
}

/*
 * Signals the completion of the usage of a semaphore
 *  - Increments the semaphore value by 1
 * Param "s": Pointer to semaphore to be signaled
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_SignalSemaphore(semaphore_t *s)
{
    //Disables interrupts
    int32_t priMask = StartCriticalSection();

    //Increment semaphore
    (*s)++;

    //Enables interrupts
    EndCriticalSection(priMask);
}

/*********************************************** Public Functions *********************************************************************/
