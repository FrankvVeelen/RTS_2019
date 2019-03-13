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

	/* Super simple, single task example */
	int i;
	for (i = NUMTASKS - 1; i >= -1; i--)
	{
		Taskp t = &Tasks[i];
		if (t->Flags & BUSY_EXEC)
		{
			break;
		}
		if (t->Activated != t->Invoked)
		{
			t->Flags |= BUSY_EXEC;
			_EINT();
			ExecuteTask(t);
			_DINT();
		}
		else {
			t->Activated = t->Invoked;
		}
	}


	/* ---------------------------------------------------------------- */
}
