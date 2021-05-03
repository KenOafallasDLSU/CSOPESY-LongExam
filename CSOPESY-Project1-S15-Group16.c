/***************************************************************
Names: TAN, DARREN
       OAFALLAS, KENNETH
Group: 16
Section: S15
***************************************************************/
#include <stdio.h>

#include "headers/queue.h"
#include "headers/process.h"
#include "headers/helpers.h"
#include "headers/mlfq.h"

void 
priorityBoost(struct Queue queuelist[], int nQueues)
{
	int i, element;
	
	for(i = 0; i < nQueues; i++)
	{
		element = dequeue(&queuelist[i + 1]);
		enqueue(element, &queuelist[0]);
	}
}

int
main()
{
	char filename[50], txt[] = ".txt";
    int nQueues, nProcesses, time_period;
    

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
    Process processlist[nProcesses]; // Processes
	int i;
	
	for(i = 0; i < nQueues; i++)
		fscanf(fp, "%d %d %d", queuelist[i].id, queuelist[i].priority, queuelist[i].quantum); 
	
	for(i = 0; i < nProcesses; i++)
		fscanf(fp, "%d %d %d %d %d", processlist[i].id, processlist[i].arrival_time, processlist[i].total_exec_time, processlist[i].burst_time_length, processlist[i].often); 
	
	priorityBoost(queuelist, nQueues);
	
	return 0;
}
