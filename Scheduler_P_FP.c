#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t)
{
  /* ----------------------- INSERT CODE HERE ----------------------- */

  t->Invoked++;
  t->Taskf(t->ExecutionTime); // execute task

  /* ---------------------------------------------------------------- */

}

void Scheduler_P_FP (Task Tasks[])
{ 
  /* ----------------------- INSERT CODE HERE ----------------------- */
  int i;
  int higherPriorityRunning = 0;
  for(i=0; i < NUMTASKS && !higherPriorityRunning; i++) {
    Taskp t = &Tasks[i];
    if (t->Activated) {
      if (!t->Invoked) {
        ExecuteTask(t);
      }
      higherPriorityRunning = 1;
    }
  }
  /* End of example*/

  /* ---------------------------------------------------------------- */
}
