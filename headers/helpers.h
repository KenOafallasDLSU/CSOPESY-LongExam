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

void printGantt(int nProcesses, struct Process aProcesses[])
{
        int systemTime = 0;
        int doneProcess[nProcesses];
        int allPrinted = 0;
        int lastEnd = 0;

        int i;
        for(i=0; i<nProcesses; i++)
                doneProcess[i] = 0;

        printf("\nGANTT CHART\n");
        do{
                //check done printing
                int allPrinted = 1;
                for(i=0; i<nProcesses; i++)
                {
                        if(aProcesses[i].runCnt != doneProcess[i])
                        {
                                allPrinted = 0;
                                break;
                        }
                        //else printf("!!!Done: %d\n", i);
                        
                        //printf("!!!Count: %d\n", doneProcess[i]);
                }
                if(allPrinted == 1) break;

                //find next
                for(i=0; i<nProcesses; i++)
                {
                        int k;
                        for(k=0; k<aProcesses[i].runCnt; k++)
                        {
                                if(aProcesses[i].aStart[k] == systemTime)
                                {
                                		doneProcess[i]++;
                                        if(aProcesses[i].aStart[k] != lastEnd)
                                        {
                                                printf("  +-------+ %d\n", lastEnd);
                                                printf("  |       |\n");
                                                printf("  | IDLE  |\n");
                                                printf("  |       |\n");
                                        }
                                        printf("  +-------+ %d\n", aProcesses[i].aStart[k]);
                                        printf("  |       |\n");
                                        printf("  | P[%d]  |\n", aProcesses[i].processID);
                                        printf("  |       |\n");

                                        lastEnd = aProcesses[i].aEnd[k];
                                }
                        }
                }

				//update time
				if(lastEnd > systemTime+1)
					systemTime = lastEnd;
				else
					systemTime++;
        } while(allPrinted == 0);
        printf("  +-------+ %d\n", lastEnd);
}
