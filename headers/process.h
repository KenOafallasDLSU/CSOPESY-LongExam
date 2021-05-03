/***********************************************
Names:  TAN, Darren
        OAFALLAS, Kenneth Neil
Group: 16
Section: S15
***********************************************/
struct Process
{
        int processID;
        int arrivalTime;
        int executionTime;
        int remainingTime;

        int ioLength;
        int ioFrequency;
        int ioCountdown;

        int prevQueue;
        int tsCountdown;

        int waitingTime;
        int turnaroundTime;

        int *aStart;
        int *aEnd;
        int *aActivity;
        int runCnt;
};

void initProcess(struct Process *p)
{
        p->waitingTime = 0;
        p->remainingTime = p->executionTime;
        p->turnaroundTime = p->executionTime;
        p->runCnt = 0;
        p->tsCountdown = -1;
        p->prevQueue = -1;
        if(p->ioLength <= 0 || p->ioFrequency <= 0)
                p->ioCountdown = -1;
        else
                p->ioCountdown = p->ioFrequency;

        p->aStart = (int*) malloc(sizeof(int) * 300);
        p->aEnd = (int*) malloc(sizeof(int) * 300);
        p->aActivity = (int*) malloc(sizeof(int) * 300);
}

void addSpace(struct Process *p)
{
//        printf("Try SPACE\n");
        p->aActivity = realloc(p->aActivity, p->runCnt+1);
        p->aStart = realloc(p->aStart, p->runCnt+1);
        p->aEnd = realloc(p->aEnd, p->runCnt+1);

//        printf("ADDING SPACE\n");
}
