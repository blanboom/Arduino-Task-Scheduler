// Scheduler.h
// A task scheduler library for arduino.
// Inspired by the book: PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS
//                       ISBN: 0-201-33138-1
// Created by Blanboom
// http://blanboom.org

#ifndef SCHEDULER_H
#define SCHEDULER_H

#if ARDUINO >= 100
#include "Arduino.h"   
#else
#include "WProgram.h"  
#endif

typedef unsigned int uint16;
typedef unsigned char uint8;

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
	uint16 delay;         // The interval before the task is first executed
	uint16 period;        // Repeat period of the task
	uint8 runMe;          // Is the task should be dispatched
	uint8 co_op;          // If it is 1, it's a co-operative task
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
	uint8 addTask(void ( *)(void), uint16, uint16, boolean);  
	boolean   deleteTask(uint8);
};

static Schedule Sch;

#endif
