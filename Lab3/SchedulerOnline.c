#ifndef __Schedule_c
#define __Schedule_c

#include "Clock.h"
#include "Scheduler.h"
#include "Led.h"
#include "Context.h"
#include "TimeTracking.h"


Task Tasks[NUMTASKS];           /* Lower indices: lower priorities           */
uint16_t NextInterruptTime;     /* Timestamp at which next interrupt should occur */

uint16_t IntDisable (void)
{
  uint16_t sw;
    // sw = r2
  asm volatile ("mov.w r2, %0\n\t" : "=r"(sw));
  _DINT();
  return (sw);
}

void RestoreSW (uint16_t sw)
{
    // r2 = sw
  asm volatile ("mov.w %0, r2\n\t" :: "r"(sw));
}  

/* 
 * Initialize and clear task structures.
 * Should be called with interrupt disabled.
 * The clock must be started elsewhere.
 */

void InitTasks (void)
{			
  uint8_t i=NUMTASKS-1; 
  do { 
    Taskp t = &Tasks[i];
    t->Flags = t->Activated = t->Invoked = 0;
  } while (i--);
}

/*
 * Register a task, TRIGGERED only, with flags.
 * Testing and filling in defaults is done.
 * Each priority level has at most one task.
 */

uint8_t RegisterTask (uint16_t Phasing, uint16_t Period, uint16_t ExecutionTime,
                      void (*TaskFunc) (uint16_t), uint8_t Prio, uint8_t Flags)
{
  uint8_t  rtc = E_SUCCESS;
  uint16_t sw;

  Phasing *= 4; // correct to get ms approximately
  Period *=4; // correct to get ms approximately

  if (Prio>=NUMTASKS) return (E_BOUNDS); // out of bounds
  if (Period == 0) return (E_WRONGPAR);
  sw = IntDisable (); 
  Taskp t = &Tasks[Prio]; 
  if (t->Flags) rtc = E_BUSY; 
  else {
    t->NextRelease = 0 + Phasing;
    t->Remaining = Phasing;
    t->Period    = Period; 
    t->NextPendingDeadline = t->NextRelease + Period;
    t->Activated = t->Invoked = 0; 
    t->Taskf     = TaskFunc; 
    t->ExecutionTime = ExecutionTime;
    t->Flags     = Flags | TRIGGERED;

  }
  RestoreSW (sw);
  return (rtc);
}

uint8_t UnRegisterTask (uint8_t t)
{
  Tasks[t].Flags = 0;
  return (E_SUCCESS);
}  

static void DetermineNextInterruptTime (CandidateValue)
{
  if (CandidateValue < NextInterruptTime)
  {
    NextInterruptTime = CandidateValue;
  }
}

interrupt (TIMERA0_VECTOR) TimerIntrpt (void)
{
  int i;
  
  StartTracking(TT_TIMER_INTERRUPT);
  
  ContextSwitch();
  
  SetLeds(WHITE, 1);

  while(NextInterruptTime <= TAR)
  {
    NextInterruptTime = 0xFFFF; // reset next intterupt time so it can be determined again

    for (i = 0; i < NUMTASKS; i++) // loop through all task to see which need to be scheduled
    {
      Taskp t = &Tasks[i];

      if (t->Flags & TRIGGERED) // check if timer available
      {
        if (t->NextRelease <= TAR) // task needs to be scheduled
        {
          t->Activated++; // set task to pending
          t->NextRelease += t->Period; // set next release time
          t->NextPendingDeadline = t->NextRelease;
        }
        DetermineNextInterruptTime(t->NextRelease); // this might be the next interrupt time
      }
    }    
  } 
 
  TACCR0 = NextInterruptTime; // set next interrupt time

  SetLeds(WHITE, 0);
  StopTracking(TT_TIMER_INTERRUPT);
  
  StartTracking(TT_SCHEDULER);
  SetLeds(BROWN, 1);

  CALL_SCHEDULER;

  SetLeds(BROWN, 0);

  ResumeContext();

  StopTracking(TT_SCHEDULER);
  PrintResults();
}

#endif

