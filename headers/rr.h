/***********************************************
Names:  TAN, Darren
        OAFALLAS, Kenneth Neil
Group: 16
Section: S15
***********************************************/
void rr(int timeShare, int nProcesses, struct Process aProcesses[])
{
	int newProcess;
        int systemTime = 0;
        int newActive = -1;
        int active = -1;
        int countdown = timeShare;
        float awt;

        struct Queue aQueue;
        initQueue(nProcesses, &aQueue);

        while(checkAllDone(nProcesses, aProcesses) == 0)
        {       
                //check new arrivals
                int loc = 0;
                do{
                	newProcess = checkNewArrival(loc, systemTime, nProcesses, aProcesses);
                	if(newProcess >= 0)
                		enqueue(newProcess, &aQueue);
                	loc = newProcess+1;
                } while(newProcess != -1);
                
                //preempt
                if(countdown == timeShare || active < 0)
                {
                        countdown = timeShare;
                        newActive = peek(&aQueue);
                        //printf("preempt %d\n", newActive);
                        if (active < 0 && newActive >= 0)
                        {
                                active = dequeue(&aQueue);
                                aProcesses[active].aStart[aProcesses[active].runCnt] = systemTime;
                                aProcesses[active].runCnt++;
                        }
                        else if(active >= 0/* && newActive >= 0*/)
                        {
                                aProcesses[active].aEnd[aProcesses[active].runCnt-1] = systemTime;
                                enqueue(active, &aQueue);

                                active = dequeue(&aQueue);
                                if(aProcesses[active].runCnt >= 1)
                                {
                                        aProcesses[active].aStart = realloc(aProcesses[active].aStart, aProcesses[active].runCnt+1);
                                        aProcesses[active].aEnd = realloc(aProcesses[active].aEnd, aProcesses[active].runCnt+1);
                                }
                                aProcesses[active].aStart[aProcesses[active].runCnt] = systemTime;
                                aProcesses[active].runCnt++;
                        } 
                }
                countdown--;
                if(countdown == 0)
                	countdown = timeShare;

                //update processs
                updateActiveProcess(&aProcesses[active]);
                int i, k;
                for(i=0; i<aQueue.size; i++)
                {
                        k = dequeue(&aQueue);
                		if(aProcesses[k].remainingTime != 0)
                            	updateWaitingProcess(&aProcesses[k]);
                        
                        enqueue(k, &aQueue);
                }

                //update system
                systemTime++;

                //handle process complete
                if(aProcesses[active].remainingTime == 0)
                {
                        countdown = timeShare;
                        aProcesses[active].aEnd[aProcesses[active].runCnt-1] = systemTime;
                        printProcessReport(&aProcesses[active]);
                        active = -1;
                }
        }

        awt = getAvgWaitingTime(nProcesses, aProcesses);
        printf("Average Waiting Time: %.1f\n", awt);
}
