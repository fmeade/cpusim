typedef struct {
    int jnumber;
    int arrivalTime;
    int burstTime;
    int timeInQueue;
    int queueChange;
    int turnaround;
    int runTime;
    int finishTime;
} Job;

typedef struct {
    int capacity;
    int size;
    int front;
    int rear;
    Job* jobs;
} Queue;

typedef struct {
    double avg_job_length;
    double avg_turnaround;
} Sim;

Job* createJob(int jnumber, int arrivalTime, int burstTime);

Queue* createQueue(int maxJobs);

void dequeue(Queue *Q);

Job* front(Queue *Q);

void enqueue(Queue *Q, Job *job);

