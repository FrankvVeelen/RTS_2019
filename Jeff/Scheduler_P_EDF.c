#include "Scheduler.h"
#include "TimeTracking.h"
#include "Led.h"

static void ExecuteTask(Taskp t)
{
	t->Invoked++;
	t->Taskf(t->ExecutionTime); // execute task
	t->Flags ^= BUSY_EXEC;
}

void Scheduler_P_EDF(Task Tasks[])
{
	uint8_t i;
	uint8_t Q[NUMTASKS - 1];

	//Init Q array Task numbers
	for (i = 0; i < NUMTASKS; i++)
	{
		Q[i] = i;
	}

	//Bubblesort on descending order
	for (i = 0; i < NUMTASKS; i++)
	{
		int j;
		for (j = 0; j < NUMTASKS - i - 1; j++)
		{
			if (Tasks[Q[j]].NextRelease > Tasks[Q[j + 1]].NextRelease)
			{
				uint8_t T = Q[j];
				Q[j] = Q[j+1];
				Q[j+1] = T;
			}
		}
	}

	for (i = 0; i < NUMTASKS; i++)
	{
		Taskp T = &Tasks[Q[i]];
		if (T->Flags & BUSY_EXEC)
		{
			break;
		}
		else
		{
			if (T->Activated != T->Invoked)
			{
				if (T->Flags & TRIGGERED)
				{
					StartTracking(TT_SCHEDULER);
					T->Flags |= BUSY_EXEC;
					_EINT();
					StopTracking(TT_SCHEDULER);
					ExecuteTask(T);
					StartTracking(TT_SCHEDULER);
					_DINT();
					StopTracking(TT_SCHEDULER);
				}
				else
				{
					T->Activated = T->Invoked;
				}
			}
		}
	}
}
