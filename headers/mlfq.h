/***********************************************
Names:  TAN, Darren
        OAFALLAS, Kenneth Neil
Group: 16
Section: S15
***********************************************/
void mlfq(struct Process aProcesses[], int nProcesses, struct Queue aQueues[], int nQueues, int boostTime)
{
  int preempt = 0;

  int newProcess;
  int systemTime = 0;
  int newActive = -1;
  int active = -1;
  int QActive = 0;
  int powerUps = 0;
  int countdown = 0;
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
// 	printf("Ordering Done\n");

  while(checkAllDone(nProcesses, aProcesses) == 0)
  {    
    //printf("Time Start: %d, Active: %d, CD: %d\n", systemTime, active, countdown);
    //check new arrivals
    int loc = 0;
    do{
      newProcess = checkNewArrival(loc, systemTime, nProcesses, aProcesses);
      if(newProcess >= 0 && aProcesses[newProcess].prevQueue < 0)
      {
        enqueue(newProcess, &(aQueues[QOrdered[0]])); //always enqueue to first queue
//        printf("%d Enqueued ", newProcess);
        if(QActive > 0) 
          preempt = 1;
    	}
      loc = newProcess+1;
    } while(newProcess != -1);

    //check IO arrivals
    loc = 0;
    do{
      newProcess = checkNewArrival(loc, systemTime, nProcesses, aProcesses);
      if(newProcess >= 0 && aProcesses[newProcess].prevQueue >= 0)
      {
        enqueue(newProcess, &(aQueues[aProcesses[newProcess].prevQueue]));
//        printf("%d Enqueued to %d", newProcess, aProcesses[newProcess].prevQueue);
        if(QActive > aProcesses[newProcess].prevQueue) 
          preempt = 1;
    	}
      loc = newProcess+1;
    } while(newProcess != -1);

    //rr process and preemption
    if(active < 0) //no active
    {
//      printf("IN IDLE\n");
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
        if(aProcesses[active].runCnt >= 300)
          addSpace(&aProcesses[active]);
        aProcesses[active].aActivity[aProcesses[active].runCnt] = aQueues[QOrdered[QActive]].id;
        aProcesses[active].aStart[aProcesses[active].runCnt] = systemTime;
        aProcesses[active].runCnt++;
      }

      if(active >= 0 && aProcesses[active].tsCountdown < 0)
      {
        aProcesses[active].tsCountdown = aQueues[QOrdered[QActive]].quantum;
        countdown = aProcesses[active].tsCountdown;
      }
      else
        countdown = aQueues[QOrdered[QActive]].quantum;
  	}
    else if(aProcesses[active].ioCountdown == 0)
    {
//      printf("IN IO\n");
      //end process
      aProcesses[active].aEnd[aProcesses[active].runCnt-1] = systemTime;

      //alloc more array space for adding IO record
      if(aProcesses[active].runCnt >= 300)
        addSpace(&aProcesses[active]);
      aProcesses[active].aActivity[aProcesses[active].runCnt] = -1;
      aProcesses[active].aStart[aProcesses[active].runCnt] = systemTime;
      aProcesses[active].aEnd[aProcesses[active].runCnt] = systemTime + aProcesses[active].ioLength;
      aProcesses[active].runCnt++;
      aProcesses[active].turnaroundTime = aProcesses[active].turnaroundTime + aProcesses[active].ioLength;

      //reset IO cd
      aProcesses[active].ioCountdown = aProcesses[active].ioFrequency;

      //demote
      if(aProcesses[active].tsCountdown == 0 && QActive < nQueues-1)
      {
        aProcesses[active].prevQueue = QActive+1;
        printf("P[%d] Demoted to %d\n", active, QActive+1);
      }
      else
        aProcesses[active].prevQueue = QActive;

      //wait for return
      if(aProcesses[active].remainingTime > 0)
        aProcesses[active].arrivalTime = aProcesses[active].ioLength + systemTime;

      if(powerUps > 0)
      {
        powerUps--;
        priorityBoost(aQueues, QOrdered, nQueues, aProcesses);
      }

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
      if(active >= 0)
      {
        if(aProcesses[active].runCnt >= 300)
          addSpace(&aProcesses[active]);
        aProcesses[active].aActivity[aProcesses[active].runCnt] = aQueues[QOrdered[QActive]].id;
        aProcesses[active].aStart[aProcesses[active].runCnt] = systemTime;
        aProcesses[active].runCnt++;
      }
      
      if(active >= 0 && aProcesses[active].tsCountdown < 0)
      {
        aProcesses[active].tsCountdown = aQueues[QOrdered[QActive]].quantum;
        countdown = aProcesses[active].tsCountdown;
      }
      else
        countdown = aQueues[QOrdered[QActive]].quantum;
    }
    else if(QActive != 0 && preempt == 1 || countdown == aQueues[QOrdered[QActive]].quantum)
    {
//      printf("IN PREEMPT\n");
      preempt--;
      //demote current process
      aProcesses[active].aEnd[aProcesses[active].runCnt-1] = systemTime;
      if(QActive+1 >= nQueues-1)
        enqueue(active, &(aQueues[QOrdered[nQueues-1]]));
      else
        enqueue(active, &(aQueues[QOrdered[QActive+1]]));

      //check if preempt
      if(QActive != 0 && aQueues[QOrdered[0]].size > 0) 
        QActive = 0;

      if(powerUps > 0)
      {
        powerUps--;
        priorityBoost(aQueues, QOrdered, nQueues, aProcesses);
      }

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
      if(aProcesses[active].runCnt >= 300)
        addSpace(&aProcesses[active]);
      aProcesses[active].aActivity[aProcesses[active].runCnt] = aQueues[QOrdered[QActive]].id;
      aProcesses[active].aStart[aProcesses[active].runCnt] = systemTime;
      aProcesses[active].runCnt++;
      
      if(active >= 0 && aProcesses[active].tsCountdown < 0)
      {
        aProcesses[active].tsCountdown = aQueues[QOrdered[QActive]].quantum;
        countdown = aProcesses[active].tsCountdown;
      }
      else
        countdown = aQueues[QOrdered[QActive]].quantum;
    }
    countdown--;
    if(countdown == 0)
      countdown = aQueues[QOrdered[QActive]].quantum;

    //update processs
    if(active >= 0)
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

    //printf("Time End: %d, Active: %d, CD: %d\n", systemTime, active, countdown);
    //update system time
    systemTime++;

    //handle process complete
    if(active >=0 && aProcesses[active].remainingTime == 0)
    {
      aProcesses[active].aEnd[aProcesses[active].runCnt-1] = systemTime;

      //alloc more array space for adding IO record
      // if(aProcesses[active].ioCountdown == 0)
      // {
      //   if(aProcesses[active].runCnt >= 300)
      //     addSpace(&aProcesses[active]);
      //   aProcesses[active].aActivity[aProcesses[active].runCnt] = -1;
      //   aProcesses[active].aStart[aProcesses[active].runCnt] = systemTime;
      //   aProcesses[active].aEnd[aProcesses[active].runCnt] = systemTime + aProcesses[active].ioLength;
      //   aProcesses[active].runCnt++;
      // }

      printProcessReport(&aProcesses[active]);
      
      active = -1;
    }

    //powerup
    if(systemTime % boostTime == 0 && systemTime > 0)
      powerUps++;

    //priorityBoost
    if(powerUps > 0 && active < 0)
    {
      powerUps--;
      priorityBoost(aQueues, QOrdered, nQueues, aProcesses);
    }
  }

  printf("Average Waiting Time: %.1f\n", getAvgWaitingTime(nProcesses, aProcesses));
}
