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
    
    valid = fscanf(fp, "%d %d %d", &nQueues, &nProcesses, &time_period); 
		if(valid < 3)
		{
			printf("ERROR: Invalid input");
			return 0;
		}
		
	if(nQueues < 2 || nQueues > 5)
	{
		printf("ERROR: Queue count out of scope");
		return 0;
	}
	
	if(nProcesses < 3 || nProcesses > 100)
	{
		printf("ERROR: Process count out of scope");
		return 0;
	}
	
	struct Queue queuelist[nQueues]; // Queues
  	struct Process processlist[nProcesses]; // Processes
	int i;
	
	for(i = 0; i < nQueues; i++)
	{
		valid = fscanf(fp, "%d %d %d", &queuelist[i].id, &queuelist[i].priority, &queuelist[i].quantum); 
		if(valid < 3)
		{
			printf("ERROR: Invalid queue input");
			return 0;
		}
		initQueue(&queuelist[i]);
	}
	
	for(i = 0; i < nProcesses; i++)
	{ 
		valid = fscanf(fp, "%d %d %d %d %d", &processlist[i].processID, &processlist[i].arrivalTime, &processlist[i].executionTime, &processlist[i].ioLength, &processlist[i].ioFrequency); 
		if(valid < 5) 
		{
			printf("ERROR: Invalid process input");
			return 0;
		}
		initProcess(&processlist[i]);
		if(processlist[i].executionTime <= 0)
		{
			i--;
			nProcesses--;
		}
	}
	
	mlfq(processlist, nProcesses, queuelist, nQueues, time_period);
	
	return 0;
}
