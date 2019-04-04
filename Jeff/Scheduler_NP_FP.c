#include "Scheduler.h"
#include "Led.h"
#include "TimeTracking.h"

static void ExecuteTask(Taskp t)
{
	t->Invoked++;
	t->Taskf(t->ExecutionTime); // execute task
	t->Flags ^= BUSY_EXEC;
}

void Scheduler_NP_FP(Task Tasks[])
{
	StartTracking(TT_SCHEDULER);
	int i;
	for (i = 0; i < NUMTASKS; i++)
	{
		Taskp t = &Tasks[i];
		if (t->Flags & BUSY_EXEC)
		{
			StopTracking(TT_SCHEDULER);
			PrintResults();
			break;
		}
		else
		{
			if (!(t->Flags & TRIGGERED))
			{
				t->Activated = t->Invoked;
				StopTracking(TT_SCHEDULER);
			}

			while (t->Activated != t->Invoked)
			{
				t->Flags |= BUSY_EXEC;
				StopTracking(TT_SCHEDULER);
				ExecuteTask(t);
				StartTracking(TT_SCHEDULER);
			}
		}
		StopTracking(TT_SCHEDULER);
		PrintResults();
	}
}

