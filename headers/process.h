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
        p->aStart = (int*) malloc(sizeof(int));
        p->aEnd = (int*) malloc(sizeof(int));
        p->aActivity = (int*) malloc(sizeof(int));
}
