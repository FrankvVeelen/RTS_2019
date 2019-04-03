#include "Scheduler.h"
#include "Led.h"
#include "TimeTracking.h"

static void ExecuteTask(Taskp t)
{
	/* ----------------------- INSERT CODE HERE ----------------------- */

	t->Invoked++;
	t->Taskf(t->ExecutionTime); // execute task
	t->Flags ^= BUSY_EXEC;
	/* ---------------------------------------------------------------- */
}

void Scheduler_NP_FP(Task Tasks[])
{
	/* ----------------------- INSERT CODE HERE ----------------------- */
	int i;
	for (i = 0; i < NUMTASKS; i++)
	{
		StartTracking(TT_SCHEDULER);
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
			}
			StopTracking(TT_SCHEDULER);
			PrintResults();

			static int Running = 0;

			while (t->Activated != t->Invoked)
			{
				if (!Running)
				{
					Running = 1;
					t->Flags |= BUSY_EXEC;
					ExecuteTask(t);
					Running = 0;
				}
			}
		}
	}
	/* ---------------------------------------------------------------- */
}

