#include "msp.h"
#include <stdint.h>
#include <driverlib.h>
#include <G8RTOS/G8RTOS.h>
#include "Threads.h"

void main(void)
{
    //Initialize G8RTOS
    G8RTOS_Init();

    //Initializing Semaphores
    G8RTOS_InitSemaphore(&LED, 1);
    G8RTOS_InitSemaphore(&Sensor, 1);

    //Adding semaphores to scheduler
    G8RTOS_AddThread(&thread0);
    G8RTOS_AddThread(&thread1);
    G8RTOS_AddThread(&thread2);

    //Start GatorOS
    G8RTOS_Launch();
}
