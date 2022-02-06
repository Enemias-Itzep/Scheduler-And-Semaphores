/*
 * Threads.h
 *
 *  Created on: Feb 12, 2021
 *      Author: enemi
 */

#ifndef THREADS_H_
#define THREADS_H_

//Declares semaphores
extern semaphore_t LED;
extern semaphore_t Sensor;


//Thread Declaration
void thread0(void);

void thread1(void);

void thread2(void);

#endif /* THREADS_H_ */
