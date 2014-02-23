This is a task scheduler for Arduinos with AVR chips and 16 MHz oscillators. Inspired by *[Patterns for Time-Triggered Embedded Systems](http://books.google.com/books?vid=ISBN0201331381&redir_esc=y&hl=zh-CN&sourceid=cndr)*.

## How to use?

**STEP 1**. Put ```Sch.init();``` and ```Sch.start();``` into ```void setup()```, and put ```Sch.dispatchTasks();``` into ```void loop()```, like this:

```C
void setup()
{
    // Your code...

    Sch.init();
    Sch.start();
}

void loop()
{
    Sch.dispatchTasks();
}
```

**STEP 2**. Put tasks at the end of your code, like this:
```C
void setup()
{
    // Your code...

    Sch.init();
    Sch.start();
}

void loop()
{
    Sch.dispatchTasks();
}

// Tasks

void task1()
{
  // Your code...
}

void task2()
{
  // Your code...
}
```

**STEP 3**. Use ```Sch.addTask()``` between ```Sch.init();``` and ```Sch.start();``` to add tasks into the task scheduler, like this:

```C
void setup()
{
    // Your code...

    Sch.init();
    
    Sch.addTask(task1,0,1000,1);  // Add task1. Starts at the 0th ms, and runs every 1000 ms
    Sch.addTask(task2,20,500,1);  // Add task2. Starts at the 20th ms, and runs every 500 ms
    
    Sch.start();
}

void loop()
{
    Sch.dispatchTasks();
}
```
中文简介在这里：http://blanboom.org/arduino-task-scheduler-library.html
