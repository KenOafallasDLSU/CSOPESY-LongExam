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
                        bAllDone = 0;

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
	        printf("Start time: %d End time: %d\n", *(process->aStart+i), *(process->aEnd+i));
        printf("Waiting time: %d\n", process->waitingTime);
        printf("Turnaround time: %d\n", process->turnaroundTime);
        printf("************************************\n");
}