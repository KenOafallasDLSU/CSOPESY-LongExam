/***********************************************
Names:  TAN, Darren
        OAFALLAS, Kenneth Neil
Group: 16
Section: S15
***********************************************/
int[] orderQueues(struct Queue aQueues[], int nQueues)
{
  int orderedQueues[];
  int i, k;
  for(i=0; i<nQueues; i++)
    orderedQueues[i] = i;
  for(i=0; i<nQueues-1;i++)
    for(k=0; k<nQueues-i-1; k++)
      if(aQueues[orderedQueues[j]].priority > aQueues[orderedQueues[j+1]].priority)
      {
        int temp = orderedQueues[j];
        orderedQueues[j] = orderedQueues[j+1];
        orderedQueues[j+1] = temp;
      }
}

void mlfq(struct Process aProcesses[], int nProcesses, struct Queue aQueues[], int nQueues)
{
  int newProcess;
  int systemTime = 0;
  int newActive = -1;
  int active = -1;
  int QActive = 0;
  int countdown;

  //order Queues
  int QOrdered[] = orderQueues(aQueue, nQueues);

  while(checkAllDone(nProcesses, aProcesses) == 0)
  {    
    //check new arrivals
    int loc = 0;
    do{
      newProcess = checkNewArrival(loc, systemTime, nProcesses, aProcesses);
      if(newProcess >= 0)
        enqueue(newProcess, &(aQueue[QOrdered[0]]); //always enqueue to first queue
      loc = newProcess+1;
    } while(newProcess != -1);

    //rr process and preemption
    if(active < 0) //no active
    {
      countdown = aQueue[QOrdered[QActive]].quantum;

      //get new active process if exists
      int i;
      for(i=0; i<nQueues; i++)
      {
        newActive = peek(&(aQueue[QOrdered[i]]));
        if(newActive >= 0)
          break;
      }
      if(newActive >= 0)
      {
        active = dequeue(&(aQueue[QOrdered[QActive]]));
        aProcesses[active].aStart[aProcesses[active].runCnt] = systemTime;
        aProcesses[active].runCnt++;
      }
    else if(QActive != 0 && orderedQueues[QActive].size > 0) || countdown == orderedQueues[QActive].quantum)
    {
      countdown = aQueue[QOrdered[QActive]].quantum;

      //demote current process
      aProcesses[active].aEnd[aProcesses[active].runCnt-1] = systemTime;
      if(QActive+1 >= nQueues-1)
        enqueue(active, &(aQueue[QOrdered[nQueues-1]]));
      else
        enqueue(active, &(aQueue[QOrdered[QActive+1]]));

      //check if preempt
      if(QActive != 0 && orderedQueues[QActive].size > 0) 
        QActive = 0;

      //get new active process
      active = dequeue(&(aQueue[QOrdered[QActive]]));
      if(aProcesses[active].runCnt >= 1)
      {
        aProcesses[active].aStart = realloc(aProcesses[active].aStart, aProcesses[active].runCnt+1);
        aProcesses[active].aEnd = realloc(aProcesses[active].aEnd, aProcesses[active].runCnt+1);
      }
      aProcesses[active].aStart[aProcesses[active].runCnt] = systemTime;
      aProcesses[active].runCnt++;
    }
    countdown--;
    if(countdown == 0)
      countdown = aQueue[QOrdered[QActive]].quantum;

    //update processs
    updateActiveProcess(&aProcesses[active]);
    int i, j, k;
    for(i=0; i<nQueues; i++)
    {
      for(j=0; j<aQueues[i].size; j++)
      {
        k = dequeue(&(aQueues+i));
        if(aProcesses[k].remainingTime != 0)
          updateWaitingProcess(&aProcesses[k]);
                          
        enqueue(k, &(aQueues+i));
      }
    }

    //update system time
    systemTime++;

    //handle process complete
    if(aProcesses[active].remainingTime == 0)
    {
      aProcesses[active].aEnd[aProcesses[active].runCnt-1] = systemTime;
      printProcessReport(&aProcesses[active]);
      active = -1;
    }
  }

  printf("Average Waiting Time: %.1f\n", getAvgWaitingTime(nProcesses, aProcesses));
}