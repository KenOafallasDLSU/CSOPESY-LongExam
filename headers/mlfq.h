/***********************************************
Names:  TAN, Darren
        OAFALLAS, Kenneth Neil
Group: 16
Section: S15
***********************************************/
void mlfq(struct Process aProcesses[], int nProcesses, struct Queue aQueues[], int nQueues)
{
  int newProcess;
  int systemTime = 0;
  int newActive = -1;
  int active = -1;
  int QActive = 0;
  int countdown;
  int i, j, k;

  //order Queues
  int QOrdered[nQueues];
  
  for(i=0; i<nQueues; i++)
    QOrdered[i] = i;
  for(i=0; i<nQueues-1;i++)
    for(k=0; k<nQueues-i-1; k++)
      if(aQueues[QOrdered[k]].priority > aQueues[QOrdered[k+1]].priority)
      {
        int temp = QOrdered[k];
        QOrdered[k] = QOrdered[k+1];
        QOrdered[k+1] = temp;
      }

//  int x;
//  for(x=0; x<nQueues; x++)
//  	printf("%d", aQueues[QOrdered[x]].id);
//	printf("Ordering Done\n");

  while(checkAllDone(nProcesses, aProcesses) == 0)
  {    
    //check new arrivals
    int loc = 0;
    do{
      newProcess = checkNewArrival(loc, systemTime, nProcesses, aProcesses);
      if(newProcess >= 0)
      {
        enqueue(newProcess, &(aQueues[QOrdered[0]])); //always enqueue to first queue
        //printf("%d Enqueued ", newProcess);
    	}
      loc = newProcess+1;
    } while(newProcess != -1);
    //printf("Arrivals at %d", systemTime);

    //rr process and preemption
    if(active < 0) //no active
    {
      //get new active process if exists
      int i;
      for(i=0; i<nQueues; i++)
      {
        newActive = peek(&(aQueues[QOrdered[i]]));
        if(newActive >= 0)
        {
        	QActive = i;
        	break;
		}
      }
      if(newActive >= 0)
      {
        active = dequeue(&(aQueues[QOrdered[QActive]]));
        aProcesses[active].aActivity[aProcesses[active].runCnt] = aQueues[QOrdered[QActive]].id;
        aProcesses[active].aStart[aProcesses[active].runCnt] = systemTime;
        aProcesses[active].runCnt++;
      }
      countdown = aQueues[QOrdered[QActive]].quantum;
      //printf("%d\n", countdown);
  	}
    else if(QActive != 0 && aQueues[QOrdered[0]].size > 0 || countdown == aQueues[QOrdered[QActive]].quantum)
    {
      //demote current process
      aProcesses[active].aEnd[aProcesses[active].runCnt-1] = systemTime;
      if(QActive+1 >= nQueues-1)
        enqueue(active, &(aQueues[QOrdered[nQueues-1]]));
      else
        enqueue(active, &(aQueues[QOrdered[QActive+1]]));

      //check if preempt
      if(QActive != 0 && aQueues[QOrdered[0]].size > 0) 
        QActive = 0;

      //get new active process
      int i;
      for(i=0; i<nQueues; i++)
      {
        newActive = peek(&(aQueues[QOrdered[i]]));
        if(newActive >= 0)
        {
        	QActive = i;
        	break;
		}
      }
      active = dequeue(&(aQueues[QOrdered[QActive]]));
      if(aProcesses[active].runCnt >= 1)
      {
        aProcesses[active].aActivity = realloc(aProcesses[active].aActivity, aProcesses[active].runCnt+1);
        aProcesses[active].aStart = realloc(aProcesses[active].aStart, aProcesses[active].runCnt+1);
        aProcesses[active].aEnd = realloc(aProcesses[active].aEnd, aProcesses[active].runCnt+1);
      }
      aProcesses[active].aActivity[aProcesses[active].runCnt] = aQueues[QOrdered[QActive]].id;
      aProcesses[active].aStart[aProcesses[active].runCnt] = systemTime;
      aProcesses[active].runCnt++;
      
      countdown = aQueues[QOrdered[QActive]].quantum;
      //printf("%d\n", countdown);
    }
    //else printf("SKIP %d\n", countdown);
    countdown--;
    if(countdown == 0)
      countdown = aQueues[QOrdered[QActive]].quantum;
    //printf(" [%d] is ACTIVE", active);

    //update processs
    updateActiveProcess(&aProcesses[active]);
    int i, j, k;
    for(i=0; i<nQueues; i++)
    {
      for(j=0; j<aQueues[i].size; j++)
      {
        k = dequeue(&aQueues[i]);
        if(aProcesses[k].remainingTime != 0)
          updateWaitingProcess(&aProcesses[k]);
                          
        enqueue(k, &aQueues[i]);
      }
    }
    //printf(" Updating at %d", systemTime);

    //update system time
    systemTime++;

    //handle process complete
    if(aProcesses[active].remainingTime == 0)
    {
      aProcesses[active].aEnd[aProcesses[active].runCnt-1] = systemTime;
      printProcessReport(&aProcesses[active]);
      active = -1;
      //printf(" Completing at %d", systemTime);
    }
    
    //printf("\n");
  }

  printf("Average Waiting Time: %.1f\n", getAvgWaitingTime(nProcesses, aProcesses));
}
