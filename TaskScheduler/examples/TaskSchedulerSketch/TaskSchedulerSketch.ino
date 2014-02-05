// Blink LEDs on digital pin 12 and 13

#include "TaskScheduler.h"  // include this file to use this library

// the state of LEDs
boolean g_led1State=1;
boolean g_led2State=0;

void setup()
{
    pinMode(13,OUTPUT);
    pinMode(12,OUTPUT);

    Sch.init();  // Initialize task scheduler

    /*
     * use Sch.addTask(task, start_time, period, priority) to add tasks
     * task - tasks to be scheduled
     * start_time - when the task starts (ms)
     * period - repeat period of the task (ms)
     * priority - 1: mormal priority, 0: high priority
     */
    Sch.addTask(led1Update,0,1000,1);  //从第 0 毫秒开始闪烁 LED，每隔 1s, LED 状态改变一次
    Sch.addTask(led2Update,20,500,1);  //从第 20 毫秒开始闪烁 LED，每隔 0.5s, LED 状态改变一次

    Sch.start();  // Start the task scheduler
}

void loop()
{
    Sch.dispatchTasks();
}

// Put task to be scheduled below

// Blink LED on pin 13
void led1Update()
{
    if(g_led1State==0)
    {
        g_led1State=1;
        digitalWrite(13,HIGH);
    }
    else
    {
        g_led1State=0;
        digitalWrite(13,LOW);
    }
}

// Blink LED on pin 12
void led2Update()
{
    if(g_led2State==0)
    {
        g_led2State=1;
        digitalWrite(12,HIGH);
    }
    else
    {
        g_led2State=0;
        digitalWrite(12,LOW);
    }
}
