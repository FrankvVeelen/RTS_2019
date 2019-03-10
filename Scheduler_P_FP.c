#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t)
{
  /* ----------------------- INSERT CODE HERE ----------------------- */

  t->Invoked++;
  t->Taskf(t->ExecutionTime); // execute task
  //When the task has completed, call the scheduler again
  CALL_SCHEDULER;

  /* ---------------------------------------------------------------- */

}

void Scheduler_P_FP (Task Tasks[])
{ 
  /* ----------------------- INSERT CODE HERE ----------------------- */
  int i;
  int higherPriorityRunning = 0;
  /*for(i=0; i < NUMTASKS && !higherPriorityRunning; i++) {
    Taskp t = &Tasks[i];
    if (t->Activated) 
	{
      if (!t->Invoked) 
	  {
        ExecuteTask(t);
      }
      higherPriorityRunning = 1;
    }
  }*/
	
	Taskp t = &Tasks[Q[Q_INDEX]];
	if (t->Activated != t->Invoked)
	{
		ExecuteTask(t);
		t->Invoked++;
	}


  /* ---------------------------------------------------------------- */
}
