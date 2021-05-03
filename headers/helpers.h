/***********************************************
Names:  TAN, Darren
        OAFALLAS, Kenneth Neil
Group: 16
Section: S15
***********************************************/
int checkAllDone(int nProcesses, struct Process aProcesses[])
{
        int bAllDone = 1;

	int i;
        for(i=0; i<nProcesses; i++)
                if(aProcesses[i].remainingTime != 0)
                {
                        bAllDone = 0;
                }        
        
        return bAllDone;
}

int checkNewArrival(int loc, int systemTime, int nProcesses, struct Process aProcesses[])
{
        int found = -1;

        int ctr = loc;
        while(found <= -1 && ctr < nProcesses)
        {
                if(aProcesses[ctr].arrivalTime == systemTime)
                        found = ctr;
                ctr++;
        }

        return found;
}

void updateActiveProcess(struct Process *process)
{
        process->remainingTime--;
        process->tsCountdown--;
        if(process->ioCountdown > 0)
                process->ioCountdown--;
}

void updateWaitingProcess(struct Process *process)
{
        process->waitingTime++;
        process->turnaroundTime++;
}

float getAvgWaitingTime(int nProcesses, struct Process aProcesses[])
{
	float sum = 0;
	
	int i;
	for(i=0; i<nProcesses; i++)
		sum += aProcesses[i].waitingTime;
	
	return sum/nProcesses;
}

void printProcessReport(struct Process *process)
{
	int i;
		
	printf("P[%d]\n", process->processID);
	for(i=0; i<process->runCnt; i++)
        {
                if(*(process->aActivity+i) >= 0)
	                printf("Q[%d] Start time: %d End time: %d\n", *(process->aActivity+i), *(process->aStart+i), *(process->aEnd+i));
                else
                        printf("[IO] Start time: %d End time: %d\n", *(process->aStart+i), *(process->aEnd+i));
        }
        printf("Waiting time: %d\n", process->waitingTime);
        printf("Turnaround time: %d\n", process->turnaroundTime);
        printf("************************************\n");
}

void priorityBoost(struct Queue aQueues[], int QOrdered[], int nQueues, struct Process aProcesses[])
{
	int i, element;
	
	for(i = 1; i < nQueues; i++)
		while(aQueues[QOrdered[i]].size > 0)
		{
		        element = dequeue(&aQueues[QOrdered[i]]);
                        aProcesses[element].tsCountdown = -1;
                        aProcesses[element].prevQueue = -1;		             
		    enqueue(element, &aQueues[QOrdered[0]]);
		}
}
