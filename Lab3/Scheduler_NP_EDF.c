#include "Scheduler.h"
#include "TimeTracking.h"
#include "Led.h"

static void ExecuteTask(Taskp t)
{
	t->Invoked++;
	t->Taskf(t->ExecutionTime); // execute task
	t->Flags ^= BUSY_EXEC;
}

void Scheduler_NP_EDF(Task Tasks[])
{
	uint8_t i;
	Taskp Q[NUMTASKS];
	uint8_t Init;

	//Init Q array of Task pointers, so we don't change the original Task array
	if (Init != 1)
	{
		for (i = 0; i < NUMTASKS; i++)
		{
			Q[i] = &Tasks[i];
		}
		Init = 1;
	}

	//Bubblesort on descending order
	for (i = 0; i < NUMTASKS; i++)
	{
		int j;
		for (j = 0; j < NUMTASKS - i - 1; j++)
		{
			if (Q[j]->NextPendingDeadline != Q[j + 1]->NextPendingDeadline)
			{
				if (Q[j]->NextPendingDeadline > Q[j + 1]->NextPendingDeadline)
				{
					Taskp T = Q[j];
					Q[j] = Q[j + 1];
					Q[j + 1] = T;
				}
			}
			else
			{
				if (Q[j+1]->Prio > Q[j]->Prio)
				{
					Taskp T = Q[j+1];
					Q[j+1] = Q[j];
					Q[j] = T;
				}
			}
		}
	}

	static uint8_t Busy = 0;

	for (i = 0; i < NUMTASKS; i++)
	{
		if (Q[i]->Flags & BUSY_EXEC)
		{
			break;
		}
		else
		{
			if (!(Q[i]->Flags & TRIGGERED))
			{
				Q[i]->Activated = Q[i]->Invoked;
			}

			if (Q[i]->Activated != Q[i]->Invoked && (Busy == 0))
			{
				StartTracking(TT_SCHEDULER);
				Q[i]->Flags |= BUSY_EXEC;
				_EINT();
				StopTracking(TT_SCHEDULER);
				Busy = 1;
				ExecuteTask(Q[i]);
				Busy = 0;
				StartTracking(TT_SCHEDULER);
				_DINT();
				StopTracking(TT_SCHEDULER);
			}
		}
	}
}
