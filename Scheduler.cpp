// Scheduler.cpp
// A task scheduler library for arduino
// Inspired by the book: PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS
//                       ISBN: 0-201-33138-1
// Created by Blanboom 
// http://blanboom.org


#include <avr/sleep.h>
#include "Scheduler.h"

uint8 g_errorCode;
Task g_Tasks[MAX_TASKS];  // Tasks array


//////////////////////////////////////////////////////////////////
// void Schedule::init(void) 
// Initialize the task scheduler.
//////////////////////////////////////////////////////////////////
void Schedule::init(void) 
{
	// Delete all tasks in the task array
	uint8 i;
	for (i = 0; i < MAX_TASKS; i++) 
	{
		deleteTask(i);
	}

	// Reset error code
	g_errorCode = 0;  

	//Set up timer1. 1ms per interrupt
	cli(); //disable global interrupt
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;
	OCR1A = 1999;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS11);  
	TIMSK1 |= (1 << OCIE1A);
}

//////////////////////////////////////////////////////////////////
// void Schedule::start(void) 
// Start the task scheduler
//////////////////////////////////////////////////////////////////
void Schedule::start(void) 
{
	sei();  //enable global interrupt
}

/////////////////////////////////////////////////////////////////
// unsigned char Schedule::addTask(void (*pFn)(), uint16 del, uint16 per, boolean co_op）
// Add a task
//
//    pFn    -    The task (function name) to be scheduled
//    del    -    The interval before the task is first executed
//    per    -    Repeat period of the task
//                If it is 0, it will run only once
//    co_op  -    If it is 1, it's a co-operative task
//                If it is 0, it's a pre-emptive task
//
// Return:  Task IDs, which can be used in deleteTask() function
//////////////////////////////////////////////////////////////////
unsigned char Schedule::addTask(void (*pFn)(), uint16 del, uint16 per, boolean co_op)  
{
	uint8 index = 0;

	// Find a gap in the array
	while ((g_Tasks[index].pTask != 0) && (index < MAX_TASKS))
	{
		index++;
	} 

	if (index == MAX_TASKS)
	{
		// The task array is full.Can't add more tasks
		g_errorCode = ERROR_TOO_MANY_TASKS;
		return MAX_TASKS; 
	}

	// Put the task into task array
	g_Tasks[index].pTask = pFn;

	g_Tasks[index].delay  = del;
	g_Tasks[index].period = per;

	g_Tasks[index].co_op = co_op;

	g_Tasks[index].runMe  = 0;

	return index; // Return task ID
}

/////////////////////////////////////////////////////////////////
// boolean Schedule::deleteTask(uint8 taskIndex)
// Delete a task
//
//     taskIndex     -   ID of the task to be deleted
//
// Return:  
//     RETURN_ERROR  -   Can't delete the task.
//     RETURN_NORMAL -   Succeed in deleting the task
//////////////////////////////////////////////////////////////////
boolean Schedule::deleteTask(uint8 taskIndex) 
{
	boolean returnCode;

	if (g_Tasks[taskIndex].pTask == 0)
	{
		// There is no task
		g_errorCode = ERROR_CANNOT_DELETE_TASK;
		returnCode = RETURN_ERROR;  // Return an error code 
	}
	else
	{
		returnCode = RETURN_NORMAL;
	}      

	g_Tasks[taskIndex].pTask   = 0;
	g_Tasks[taskIndex].delay   = 0;
	g_Tasks[taskIndex].period  = 0;

	g_Tasks[taskIndex].runMe   = 0;

	return returnCode;       // return status
}

/////////////////////////////////////////////////////////////////
// void Schedule::dispatchTasks(void) 
// Run tasks
/////////////////////////////////////////////////////////////////
void Schedule::dispatchTasks(void) 
{
	uint8 index;
	for (index = 0; index < MAX_TASKS; index++)
	{
		// Run co-operative tasks only
		if ((g_Tasks[index].co_op) && (g_Tasks[index].runMe > 0)) 
		{
			(*g_Tasks[index].pTask)();  // Run the task

			g_Tasks[index].runMe -= 1;   

			// If period = 0, this task will only run once
			if (g_Tasks[index].period == 0)
			{
				g_Tasks[index].pTask = 0;  // Delete the task
			}
		}
	}
	_reportStatus();  // Report errors (if necessary)
	_goToSleep();  // Go to sleep (idle) mode
}

//////////////////////////////////////////////////////////////////
// void Schedule::_reportStatus(void)
// Report errors
// Define REPORT_ERRORS and error codes in "Scheduler.h" first
//////////////////////////////////////////////////////////////////
void Schedule::_reportStatus(void)
{
#ifdef REPORT_ERRORS
	// Put you codes to report errors
#endif
}


//////////////////////////////////////////////////////////////////
// void Schedule::_goToSleep()
// Enable sleep mode.
//////////////////////////////////////////////////////////////////
void Schedule::_goToSleep()
{
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();   
	sleep_mode();
}

//////////////////////////////////////////////////////////////////
// ISR (TIMER1_COMPA_vect)
// Update task informations and run pre-emptive tasks
//////////////////////////////////////////////////////////////////
ISR (TIMER1_COMPA_vect)
{
	uint8 index;  
	for (index = 0; index < MAX_TASKS; index++)
	{
		// Check if there is a task
		if (g_Tasks[index].pTask)
		{
			if (g_Tasks[index].delay == 0)
			{
				if (g_Tasks[index].co_op)
				{
					// A co-operative task
					g_Tasks[index].runMe += 1;  
				}
				else
				{
					// A pre-emptive task 
					(*g_Tasks[index].pTask)();  // Run the task now

					g_Tasks[index].runMe -= 1;   

					// Remove the task that the period = 0
					if (g_Tasks[index].period == 0)
					{
						g_Tasks[index].pTask  = 0;
					}
				}

				if (g_Tasks[index].period)
				{
					// Schedule the task to run again
					g_Tasks[index].delay = g_Tasks[index].period;
				}
			}
			else
			{
				// Decrease the delay
				g_Tasks[index].delay -= 1;
			}
		}         
	}
}   