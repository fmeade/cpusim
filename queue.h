typedef struct {
    int jnumber;
    int arrivalTime;
    int burstTime;
    int timeInQueue;
    int currentQueue;
} Job;

typedef struct {
    int capacity;
    int size;
    int front;
    int rear;
    Job *jobs;
} Queue;

Queue* createQueue(int maxJobs);

void Dequeue(Queue *Q);

Job front(Queue *Q);

void Enqueue(Queue *Q, Job job);

