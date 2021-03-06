#include "Scheduler.h"
#include "TimeTracking.h"
#include "Led.h"

static void ExecuteTask(Taskp t)
{
	t->Invoked++;
	t->Taskf(t->ExecutionTime); // execute task
}

void Scheduler_NP_EDF(Task Tasks[])
{
	StartTracking(TT_SCHEDULER);
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
				Q[j] = Q[j + 1];
				Q[j + 1] = T;
			}
		}
	}
	StopTracking(TT_SCHEDULER);
	for (i = 0; i < NUMTASKS; i++)
	{
		Taskp T = &Tasks[Q[i]];
		if (T->Flags & BUSY_EXEC )
		{
			StopTracking(TT_SCHEDULER);
			break;
		}
		else
		{
			while (T->Activated != T->Invoked)
			{
				if (T->Flags & TRIGGERED && !Running)
				{
					int j;
					StopTracking(TT_SCHEDULER);
					ExecuteTask(T);
					StartTracking(TT_SCHEDULER);
				}
				else
				{
					T->Activated = T->Invoked;
				}
			}
		}
		StopTracking(TT_SCHEDULER);
		PrintResults();
	}
}
