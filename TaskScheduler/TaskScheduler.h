// Scheduler.h
// A task scheduler library for arduino - header file
// http://blanboom.org

#ifndef SCHEDULER_H
#define SCHEDULER_H

#if ARDUINO >= 100
#include "Arduino.h"   
#else
#include "WProgram.h"  
#endif


#define RETURN_ERROR 0
#define RETURN_NORMAL 1

// Error codes
//#define REPORT_ERRORS // Remove "//" to enable error report
#define ERROR_TOO_MANY_TASKS (1)
#define ERROR_CANNOT_DELETE_TASK (2)

// The max number of tasks
#define MAX_TASKS   (10) 

// The data structure of per task
typedef struct 
{
	void (*pTask)(void);  // The task to be scheduled
	uint16_t delay;         // The interval before the task is first executed
	uint16_t period;        // Repeat period of the task
	uint8_t runMe;          // Is the task should be dispatched
	uint8_t co_op;          // If it is 1, it's a co-operative task
	// If it is 0, it's a pre-emptive task
} Task;   

class Schedule
{
	//Scheduler functions
private:
	void _reportStatus(void);
	void _goToSleep(void);
public:
	void init(void);
	void start(void);
	void  dispatchTasks(void);
	uint8_t addTask(void ( *)(void), uint16_t, uint16_t, boolean);  
	boolean   deleteTask(uint8_t);
};

static Schedule Sch;

#endif
