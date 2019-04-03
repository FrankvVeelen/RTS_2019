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

	for (i = 0; i < NUMTASKS; i++)
	{
		Taskp T = &Tasks[Q[i]];
		if (T->Flags & BUSY_EXEC )
		{
			break;
		}
		else
		{
			static int Running = 0;
			while (T->Activated != T->Invoked)
			{
				if (T->Flags & TRIGGERED && !Running)
				{
					int j;
					Running = 1;
					StartTracking(TT_SCHEDULER);
					//_EINT();
					StopTracking(TT_SCHEDULER);
					ExecuteTask(T);
					StartTracking(TT_SCHEDULER);
					//_DINT();
					StopTracking(TT_SCHEDULER);
					Running = 0;
				}
				else
				{
					T->Activated = T->Invoked;
				}
			}
		}
	}
}
