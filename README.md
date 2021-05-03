# CSOPESY Long Exam 1 - MLFQ in C

## Authors

CSOPESY - S15 - Group 16

- TAN, Darren

- OAFALLAS, Kenneth Neil

## Additional Instructions

When the program is executed, a prompt
```
Enter file name:
```
will ask for the file name of a txt file. The input file name should be without the file extension `.txt` as the program automatically appends the extension.

Thus, the file `sample.txt` should be input as

```
Enter file name: sample
```

## Notes on Behavior

- Priority Boost enqueues processes to the highest priority queue in FCFS order by repeated dequeue-enqueue, from higest priority to lowest priority, such that the priority and order is retained.

- A process that has consumed its time share is requeued first before IO processes are requeued if they occur at the same time.

- Preempted processes, processes that have consumed their time share, and returning IO processes are requeued first before Priority Boost is performed.

- Newly arriving processes are enqueued first before preempted processes and processes that have consumed their time share.

- IO time does not count to waiting time, but is counted in turnaround time



