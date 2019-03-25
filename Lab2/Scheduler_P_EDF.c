#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t)
{
  t->Invoked++;
  t->Taskf(t->ExecutionTime); // execute task
  t->Flags ^= BUSY_EXEC;
}

void Scheduler_P_EDF (Task Tasks[])
{
  int i;
  int firstDeadline;
  int firstDeadlineTask;
  for (i = 0; i < NUMTASKS; i++)
  {
    StartTracking(TT_SCHEDULER);
    Taskp t = &Tasks[i];
    if(i == 0){
      firstDeadline = t->NextPendingDeadline;
      firstDeadlineTask = 0;
    }
    else if(t->NextPendingDeadline < firstDeadline){
      firstDeadline = t->NextPendingDeadline;
      firstDeadlineTask = i;
    }
  }
  Taskp t = &Tasks[firstDeadlineTask];

  StartTracking(TT_SCHEDULER);
  t->Flags |= BUSY_EXEC;
  _EINT();
  StopTracking(TT_SCHEDULER);
  ExecuteTask(t);
  StartTracking(TT_SCHEDULER);
  _DINT();
  StopTracking(TT_SCHEDULER);
}
