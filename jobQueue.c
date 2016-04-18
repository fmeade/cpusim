#include <stdio.h>
#include <stdlib.h>

#include "jobQueue.h"

Job* createJob(int jnumber, int arrivalTime, int burstTime) {
    
    /* Create a Job */
    Job *job;
    job = (Job *)malloc(sizeof(Job));

    /* Intialize its properties */
    job->jnumber = jnumber;
    job->arrivalTime = arrivalTime;
    job->burstTime = burstTime;
    job->timeInQueue = 0;
    job->queueChange = 0;
    job->turnaround = 0;
    job->finishTime = 0;

    return job;  
}

Queue* createQueue(int maxJobs) {
    
    /* Create a Queue */
    Queue *Q;
    Q = (Queue *)malloc(sizeof(Queue));

    /* Initialize its properties */
    Q->jobs = (Job *)malloc(sizeof(int)*maxJobs);
    Q->size = 0;
    Q->capacity = maxJobs;
    Q->front = 0;
    Q->rear = -1;

    /* Return the pointer */
    return Q; 
}

void Dequeue(Queue *Q)
{
    /* If Queue size is zero then it is empty. So we cannot pop */
    if(Q->size == 0)
    {
        perror("Queue is Empty\n");
        return;
    }
    /* Removing an element is equivalent to incrementing index of front by one */
    else
    {
        Q->size--;
        Q->front++;
                /* As we fill elements in circular fashion */
        if(Q->front == Q->capacity)
        {
            Q->front = 0;
        }
    }

    return;
}

Job front(Queue *Q)
{
    if(Q->size == 0)
    {
        perror("Queue is Empty\n");
    }

    /* Return the element which is at the front*/
    return Q->jobs[Q->front];
}

void Enqueue(Queue *Q, Job job)
{
    /* If the Queue is full, we cannot push an element into it as there is no space for it.*/
    if(Q->size == Q->capacity)
    {
        perror("Queue is Full\n");
    }
    else
    {
        Q->size++;
        Q->rear = Q->rear + 1;
        /* As we fill the queue in circular fashion */
        if(Q->rear == Q->capacity)
        {
            Q->rear = 0;
        }
        /* Insert the element in its rear side */ 
        Q->jobs[Q->rear] = job;
    }

    return;
}