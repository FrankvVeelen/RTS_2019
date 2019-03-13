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

            while (t->Activated != t->Invoked)
			{
                StartTracking(TT_SCHEDULER);
                t->Flags |= BUSY_EXEC;
                _EINT();
                StopTracking(TT_SCHEDULER);
                ExecuteTask(t);
                StartTracking(TT_SCHEDULER);
                _DINT();
                StopTracking(TT_SCHEDULER);
            }
        }
    }
	/* ---------------------------------------------------------------- */
}
