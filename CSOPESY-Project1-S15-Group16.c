/***************************************************************
Names: TAN, DARREN
       OAFALLAS, KENNETH
Group: 16
Section: S15
***************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/queue.h"
#include "headers/process.h"
#include "headers/helpers.h"
#include "headers/mlfq.h"

// void 
// priorityBoost(struct Queue queuelist[], int nQueues)
// {
// 	int i, element;
	
// 	for(i = 0; i < nQueues; i++)
// 	{
// 		element = dequeue(&queuelist[i + 1]);
// 		enqueue(element, &queuelist[0]);
// 	}
// }

int
main()
{
	char filename[50], txt[] = ".txt";
    int nQueues, nProcesses, time_period;
	int valid;
    

    printf("Enter file name: ");
    scanf("%s", filename);
	printf("\n");
	
    strcat(filename, txt);

    FILE* fp = fopen(filename, "r");

    if(fp == NULL)
    {
        printf("%s not found.", filename);
        return 0;
    }
    
    fscanf(fp, "%d %d %d", &nQueues, &nProcesses, &time_period); 
	
	struct Queue queuelist[nQueues]; // Queues
  	struct Process processlist[nProcesses]; // Processes
	int i;
	
	for(i = 0; i < nQueues; i++)
	{
		valid = fscanf(fp, "%d %d %d", &queuelist[i].id, &queuelist[i].priority, &queuelist[i].quantum); 
		if(valid < 3) return 0;
		initQueue(&queuelist[i]);
	}
	
	for(i = 0; i < nProcesses; i++)
	{
		//valid = fscanf(fp, "%d %d %d %d %d", processlist[i].processID, processlist[i].arrival_time, processlist[i].total_exec_time, processlist[i].burst_time_length, processlist[i].often); 
		valid = fscanf(fp, "%d %d %d %d %d", &processlist[i].processID, &processlist[i].arrivalTime, &processlist[i].executionTime, &processlist[i].ioLength, &processlist[i].ioFrequency); 
		if(valid < 5) return 0;
		initProcess(&processlist[i]);
		if(processlist[i].executionTime <= 0)
		{
			i--;
			nProcesses--;
		}
	}
	
	mlfq(processlist, nProcesses, queuelist, nQueues);
	//priorityBoost(queuelist, nQueues);
	
	return 0;
}
