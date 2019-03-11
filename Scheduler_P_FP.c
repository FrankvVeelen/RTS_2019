#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t)
{
  /* ----------------------- INSERT CODE HERE ----------------------- */

  t->Taskf(t->ExecutionTime); // execute task
  t->Invoked++;

  /* ---------------------------------------------------------------- */

}

void Scheduler_P_FP (Task Tasks[])
{ 
  /* ----------------------- INSERT CODE HERE ----------------------- */
  int i;
  for(i=NUMTASKS-1; i!=-1; i--) {
	  Taskp t = &Tasks[i];
	  if (t->Activated != t->Invoked) {
		  ExecuteTask(t);
      }
  }

  /* ---------------------------------------------------------------- */
}
