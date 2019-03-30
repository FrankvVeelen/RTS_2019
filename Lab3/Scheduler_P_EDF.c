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
	Taskp Q[NUMTASKS];
	uint8_t Init;

	//Init Q array of Task pointers, so we don't change the original Task array
	/*if (Init != 1)
	{
		for (i = 0; i < NUMTASKS; i++)
		{
			Q[i] = &Tasks[i];
		}
		Init = 1;
	}*/

	//Bubblesort on descending order
	/*for (i = 0; i < NUMTASKS; i++)
	{
		int j;
		for (j = 0; j < NUMTASKS - i - 1; j++)
		{
			if (Q[j]->NextPendingDeadline != Q[j + 1]->NextPendingDeadline)
			{
				if (Q[j]->NextPendingDeadline > Q[j + 1]->NextPendingDeadline)
				{
					Taskp T = Q[j+1];
					Q[j+1] = Q[j];
					Q[j] = T;
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
	}*/

	//Bubblesort on descending order
	for (i = 0; i < NUMTASKS; i++)
	{
		int j;
		for (j = 0; j < NUMTASKS - i - 1; j++)
		{
			if (Tasks[j].NextRelease > Tasks[j + 1].NextRelease)
			{
				Task T = Tasks[j + 1];
				if (Tasks[j].NextRelease == Tasks[j + 1].NextRelease)
				{
					//T.Flags |= BUSY_EXEC;

					if (Tasks[j].Prio > Tasks[j + 1].Prio)
					{
						Tasks[j + 1] = Tasks[j];
						Tasks[j] = T;
					}
				}

				Tasks[j + 1] = Tasks[j];
				Tasks[j] = T;
			}
		}
	}

	/*for (i = 0; i < NUMTASKS; i++)
	{
		if (Tasks[i].NextRelease == Tasks[i + 1].NextRelease)
		{
			Task T = Tasks[i + 1];
			T.Flags |= BUSY_EXEC;

			if (Tasks[i].Prio > Tasks[i + 1].Prio)
			{
				Tasks[i + 1] = Tasks[i];
				Tasks[i] = T;
			}
		}
	}*/

/*	for (i = 0; i < NUMTASKS; i++)
	{
		if (Q[i]->Flags & BUSY_EXEC)
		{
			break;
		}
		else
		{
			if (Q[i]->Activated != Q[i]->Invoked)
			{
				if (Q[i]->Flags & TRIGGERED)
				{
					StartTracking(TT_SCHEDULER);
					Q[i]->Flags |= BUSY_EXEC;
					_EINT();
					StopTracking(TT_SCHEDULER);
					ExecuteTask(Q[i]);
					StartTracking(TT_SCHEDULER);
					_DINT();
					StopTracking(TT_SCHEDULER);
				} 
				else 
				{
					Q[i]->Activated = Q[i]->Invoked;
				}
			}
		}
	}*/

	for (i = 0; i < NUMTASKS; i++)
	{
		if (Tasks[i].Flags & BUSY_EXEC)
		{
			break;
		}
		else
		{
			if (Tasks[i].Activated != Tasks[i].Invoked)
			{
				if (Tasks[i].Flags & TRIGGERED)
				{
					StartTracking(TT_SCHEDULER);
					Tasks[i].Flags |= BUSY_EXEC;
					_EINT();
					StopTracking(TT_SCHEDULER);
					ExecuteTask(&Tasks[i]);
					StartTracking(TT_SCHEDULER);
					_DINT();
					StopTracking(TT_SCHEDULER);
				}
				else
				{
					Tasks[i].Activated = Tasks[i].Invoked;
				}
			}
		}
	}
}
