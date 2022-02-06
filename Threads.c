/*
 * Threads.c
 *
 *  Created on: Feb 12, 2021
 *      Author: enemi
 */
#include <BSP.h>
#include <driverlib.h>
#include <G8RTOS/G8RTOS_Semaphores.h>
#include "Threads.h"

void thread0(void)
{
    while(1)
    {
        //Waits for sensor I2C semaphore
        G8RTOS_WaitSemaphore(&Sensor);

        //Reads accelerometer
        int16_t accelX;
        while(bmi160_read_accel_x(&accelX));

        //Releases Sensor
        G8RTOS_SignalSemaphore(&Sensor);

        //Interprets accelerometer data
        accelX *= -1;

        uint16_t temp;

        if(accelX > 14000)
        {
            temp = 0xFF00;
        }
        else if(accelX > 120000)
        {
            temp = 0x7F00;
        }
        else if(accelX > 10000)
        {
            temp = 0x3F00;
        }
        else if(accelX > 8000)
        {
            temp = 0x1F00;
        }
        else if(accelX > 6000)
        {
            temp = 0x0F00;
        }
        else if(accelX > 4000)
        {
            temp = 0x0700;
        }
        else if(accelX > 2000)
        {
            temp = 0x0300;
        }
        else if(accelX > 0)
        {
            temp = 0x0100;
        }
        else if(accelX > -2000)
        {
            temp = 0x0080;
        }
        else if(accelX > -4000)
        {
            temp = 0x00C0;
        }
        else if(accelX > -6000)
        {
            temp = 0x00E0;
        }
        else if(accelX > -8000)
        {
            temp = 0x00F0;
        }
        else if(accelX > -10000)
        {
            temp = 0x00F8;
        }
        else if(accelX > -12000)
        {
            temp = 0x00FC;
        }
        else if(accelX > -14000)
        {
            temp = 0x00FE;
        }
        else
        {
            temp = 0x00FF;
        }

        //Waits for LED semaphore
        G8RTOS_WaitSemaphore(&LED);

        //Outputs on LEDs
        LP3943_LedModeSet(RED, temp);

        //Releases LED
        G8RTOS_SignalSemaphore(&LED);

        uint32_t i;
        for (i = 0; i < 48000; i++);
    }

}

void thread1(void)
{
    while(1)
    {
        //Waits for sensor I2C semaphore
        G8RTOS_WaitSemaphore(&Sensor);

        //Reads light sensor data
        uint16_t lightData;
        while(!sensorOpt3001Read(&lightData));

        //Releases Sensor
        G8RTOS_SignalSemaphore(&Sensor);

        //Interprets light sensor data
        uint16_t temp;

        if(lightData > 49000)
        {
            temp = 0xFFFF;
        }
        else if(lightData > 45500)
        {
            temp = 0x7FFF;
        }
        else if(lightData > 42000)
        {
            temp = 0x3FFF;
        }
        else if(lightData > 38500)
        {
            temp = 0x1FFF;
        }
        else if(lightData > 35500)
        {
            temp = 0x0FFF;
        }
        else if(lightData > 31500)
        {
            temp = 0x07FF;
        }
        else if(lightData > 28000)
        {
            temp = 0x03FF;
        }
        else if(lightData > 26000)
        {
            temp = 0x01FF;
        }
        else if(lightData > 24500)
        {
            temp = 0x00FF;
        }
        else if(lightData > 21000)
        {
            temp = 0x007F;
        }
        else if(lightData > 17500)
        {
            temp = 0x003F;
        }
        else if(lightData > 14000)
        {
            temp = 0x001F;
        }
        else if(lightData > 10500)
        {
            temp = 0x000F;
        }
        else if(lightData > 7000)
        {
            temp = 0x0007;
        }
        else if(lightData > 3500)
        {
            temp = 0x0003;
        }
        else
        {
            temp = 0x0001;
        }

        //Waits for LED semaphore
        G8RTOS_WaitSemaphore(&LED);

        //Outputs on LEDs
        LP3943_LedModeSet(GREEN, temp);

        //Releases LED
        G8RTOS_SignalSemaphore(&LED);

        uint32_t i;
        for (i = 0; i < 48000; i++);
    }
}

void thread2(void)
{
    while (1)
    {
        //Waits for sensor I2C semaphore
        G8RTOS_WaitSemaphore(&Sensor);

        //Reads z-axis sensor data
        int16_t gyroZ;
        while(bmi160_read_gyro_z(&gyroZ));

        //Releases Sensor
        G8RTOS_SignalSemaphore(&Sensor);

        //Interprets gyroscope data
        gyroZ *= -1;

        uint16_t temp;

        if(gyroZ > 7000)
        {
            temp = 0xFF00;
        }
        else if(gyroZ > 6000)
        {
            temp = 0x7F00;
        }
        else if(gyroZ > 5000)
        {
            temp = 0x3F00;
        }
        else if(gyroZ > 4000)
        {
            temp = 0x1F00;
        }
        else if(gyroZ > 3000)
        {
            temp = 0x0F00;
        }
        else if(gyroZ > 2000)
        {
            temp = 0x0700;
        }
        else if(gyroZ > 1000)
        {
            temp = 0x0300;
        }
        else if(gyroZ > 0)
        {
            temp = 0x0100;
        }
        else if(gyroZ > -1000)
        {
            temp = 0x0080;
        }
        else if(gyroZ > -2000)
        {
            temp = 0x00C0;
        }
        else if(gyroZ > -3000)
        {
            temp = 0x00E0;
        }
        else if(gyroZ > -4000)
        {
            temp = 0x00F0;
        }
        else if(gyroZ > -5000)
        {
            temp = 0x00F8;
        }
        else if(gyroZ > -6000)
        {
            temp = 0x00FC;
        }
        else if(gyroZ > -7000)
        {
            temp = 0x00FE;
        }
        else
        {
            temp = 0x00FF;
        }

        //Waits for LED semaphore
        G8RTOS_WaitSemaphore(&LED);

        //Outputs on LEDs
        LP3943_LedModeSet(BLUE, temp);

        //Releases LED
        G8RTOS_SignalSemaphore(&LED);

        uint32_t i;
        for (i = 0; i < 48000; i++);
    }
}
