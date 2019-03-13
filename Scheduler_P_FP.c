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

void Scheduler_P_FP(Task Tasks[])
{
	/* ----------------------- INSERT CODE HERE ----------------------- */

	/* Super simple, single task example */
	int i;
	//for (i = NUMTASKS - 1; i >= -1; i--)
	for (i = 0; i < NUMTASKS; i++)
	{
		Taskp t = &Tasks[i];
		if (t->Flags & BUSY_EXEC)
		{
			break;
		}
		else
		{
			if (!(t->Flags & TRIGGERED))
			{
				t->Activated = t->Invoked;
			}

			while (t->Activated != t->Invoked)
			{
				t->Flags |= BUSY_EXEC;
				_EINT();
				StopTracking(TT_TIMER_INTERRUPT);
				ExecuteTask(t);
				StartTracking(TT_TIMER_INTERRUPT);
				_DINT();
			}
		}
	}
	/* ---------------------------------------------------------------- */
}
