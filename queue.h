/***********************************************
Names:  TAN, Darren
        OAFALLAS, Kenneth Neil
Group: 16
Section: S15
***********************************************/
struct Queue
{
    int *elements;
    int size;
    int max;
    int head;
    int tail;
    int id;
	int priority;
	int quantum;
};

void initQueue(int n, struct Queue *q)
{
	q->elements = (int*)malloc(n * sizeof(int));
	q->max = n;
	q->size = 0;
    q->head = 0;
    q->tail = n-1;
}

int peek(struct Queue *q)
{
	if(q->size <= 0) return -1;
	//printf("%d\n",q->elements[q->head]);
    return q->elements[q->head];
}

int dequeue(struct Queue *q)
{
        if(q->size <= 0) return -1;
        
        int element;
        element = q->elements[q->head];
        q->head = (q->head+1) % q->max;
        q->size--;
        //printf("DQ: %d, Size: %d, T: %d, H: %d\n", element, q->size, q->tail, q->head);
        return element;
}

void enqueue(int element, struct Queue *q)
{
		if(q->size >= q->max) return;
        q->tail = (q->tail+1) % q->max;
        q->elements[q->tail] = element;
        q->size++;
        //printf("NQ: %d, Size: %d, T: %d, H: %d\n", q->elements[q->tail], q->size, q->tail, q->head);
}
