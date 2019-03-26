#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask(Taskp t)
{
	/* ----------------------- INSERT CODE HERE ----------------------- */

	t->Invoked++;
	t->Taskf(t->ExecutionTime); // execute task
	t->Flags ^= BUSY_EXEC;
	/* ---------------------------------------------------------------- */
}

void Scheduler_P_FP(Task Tasks[])
{
	/* ----------------------- INSERT CODE HERE ----------------------- */
	int i;
	static bool Busy;
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

			while (t->Activated != t->Invoked && !Busy)
			{
				StartTracking(TT_SCHEDULER);
				t->Flags |= BUSY_EXEC;
				_EINT();
				StopTracking(TT_SCHEDULER);
				Busy = true;
				ExecuteTask(t);
				Busy = false;
				StartTracking(TT_SCHEDULER);
				_DINT();
				StopTracking(TT_SCHEDULER);
			}
		}
	}
	/* ---------------------------------------------------------------- */
}
