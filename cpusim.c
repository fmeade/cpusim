#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#include "command-line-options.h"
#include "ibarland-utils.h"
#include "jobQueue.h"

/**
 * cpusim.c
 * 
 * Project 4 - Process Scheduling
 * Author: Forrest Meade (fmeade)
 * Due: April 19, 2016 @ 2p
 *
 * Description: Design and implement a program that simulates a CPU scheduler. 
 *              We will explore one particular scheduling algorithm (multilevel 
 *              queueing). You will gain experience in both writing a simulation 
 *              program1, and in the high-level concepts behind various CPU scheduling 
 *              algorithms.
 * 
 * Help: Queues - http://www.thelearningpoint.net/computer-science/data-structures-queues--with-c-program-source-code
 * 
 */

 /* The possible command-line options to the program. 
 */
 struct option_info options[] =
 {  { "numJobs"             , 'n', "8", "the number of jobs to complete, before ending the simulation. " }
 ,{ "foreground-quantum"  , 'f', "4", "the time (in ms) jobs in the foreground queue get, " 
 "before being moved to the intermediate queue. " } 
 ,{ "intermediate-quantum", 'i', "8", "the time (in ms) jobs in the intermediate queue get, " 
 "before being moved to the background queue. " } 
 ,{ "job-length"          , 'l', "4", "the expected job-length in ms. " } 
 ,{ "job-frequency"       , 'r', "1", "the expected number of new jobs per ms. " } 
 ,{ "seed"                , 's', NULL, "The random-number seed for the simulation. " } 
 ,{ "verbose"             , 'v', "true", "enable verbose output or not. " } 
};

#define NUM_OPTIONS SIZEOF_ARRAY(options)


int main(int argc, char** argv) {

	char** settings = allOptions( argc, argv, NUM_OPTIONS, options );
    // Now, the array `settings` contains all the options, in order:
    // either taken from the command-line, or from the default given in `options[]`.

	const int NUMJOBS = atoi(settings[0]);
	const int FOREGROUND_QUANTUM = atoi(settings[1]);
	const int INTERMEDIATE_QUANTUM = atoi(settings[2]);
	const int JOB_LENGTH = atoi(settings[3]);
	const int JOB_FREQUENCY = atoi(settings[4]);


    int SEED;

    // generates a random number for the seed if not provided
    srandom(time(NULL));
    if (settings[5] == NULL)
        SEED = random();
        //Now, the seed is a random integer
    else
        SEED = atoi(settings[5]);



    bool VERBOSE;
    if (settings[6][0] == 't')
        VERBOSE = true;
    else
        VERBOSE = false;


    Sim* sim;
    sim = (Sim *)malloc(sizeof(Sim));

    sim->avg_job_length = 0.0;
    sim->avg_turnaround = 0.0;


    long sim_start_time = time_usec() / 1000; // current time in ms




    Job* jobs[NUMJOBS];
    /* Fills array with initialized Jobs */
    int counter = 0;
    int arrival = 0;
    while (counter != SIZEOF_ARRAY(jobs)) {

        for (int i = 0; i < JOB_FREQUENCY; i++) {

            jobs[counter] = createJob(counter+1, arrival, (rand() % ((JOB_LENGTH + 3) - (JOB_LENGTH - 1))) + (JOB_LENGTH - 1));
            sim->avg_job_length = sim->avg_job_length + jobs[counter]->burstTime;
            counter++;
        }
        arrival++;
    }



    Queue* foregroundQueue = createQueue(SIZEOF_ARRAY(jobs));
    Queue* intermediateQueue = createQueue(SIZEOF_ARRAY(jobs));
    Queue* backgroundQueue = createQueue(SIZEOF_ARRAY(jobs));




    int frontJobIndex;

    /* loops through a calculated number of milliseconds */
    for (int i = 0; i < NUMJOBS*(JOB_LENGTH*JOB_LENGTH); i++) {

        /* Puts jobs into foreground queue that arrive at time i */
        for (int j = 0; j < SIZEOF_ARRAY(jobs); j++) {

            if(jobs[j]->arrivalTime == i) {
                enqueue(foregroundQueue, jobs[j]);
                jobs[j]->queueChange++;

            }
        }

        /* Handles the foreground queue if not empty */
        if (foregroundQueue->size != 0) {

            frontJobIndex = front(foregroundQueue)->jnumber - 1;

            /* Moves all appropriate counters up one */
            for (int j = 0; j < SIZEOF_ARRAY(jobs); j++) {

                if(jobs[j]->queueChange > -1 && jobs[j]->queueChange < 3) {

                    jobs[j]->timeInQueue++;

                    /* Increases only if its still in a queue */
                    if(jobs[j]->runTime != jobs[j]->burstTime)
                        jobs[j]->finishTime++;
                }
            }

            jobs[frontJobIndex]->runTime++;


            if(jobs[frontJobIndex]->runTime == jobs[frontJobIndex]->burstTime)
                dequeue(foregroundQueue);



            if (foregroundQueue->size != 0) {
                frontJobIndex = front(foregroundQueue)->jnumber - 1;


                /* Moves all jobs that are at the foreground quantum to the intermediate queue */
                while(jobs[frontJobIndex]->timeInQueue == FOREGROUND_QUANTUM) {
                    dequeue(foregroundQueue);
                    jobs[frontJobIndex]->timeInQueue = 0;
                    jobs[frontJobIndex]->queueChange++;
                    enqueue(intermediateQueue, jobs[frontJobIndex]);


                    if (foregroundQueue->size != 0) {
                        frontJobIndex = front(foregroundQueue)->jnumber - 1;
                    }
                }

                /* Moves all jobs that are at the intermediate quantum to the background queue */
                if (intermediateQueue->size != 0) {
                    frontJobIndex = front(intermediateQueue)->jnumber - 1;


                    while(jobs[frontJobIndex]->timeInQueue == INTERMEDIATE_QUANTUM) {
                        dequeue(intermediateQueue);
                        jobs[frontJobIndex]->timeInQueue = 0;
                        jobs[frontJobIndex]->queueChange++;
                        enqueue(backgroundQueue, jobs[frontJobIndex]);


                        if (intermediateQueue->size != 0) {
                            frontJobIndex = front(intermediateQueue)->jnumber - 1;
                        }
                    }
                }
            }
        }
        else {
            /* Handles intermediate queue if not empty */
            if (intermediateQueue->size != 0) {

                frontJobIndex = front(intermediateQueue)->jnumber - 1;

                /* Moves all appropriate counters up one */
                for (int j = 0; j < SIZEOF_ARRAY(jobs); j++) {

                    if(jobs[j]->queueChange == 1 || jobs[j]->queueChange == 2) {

                        jobs[j]->timeInQueue++;

                        /* Increases only if its still in a queue */
                        if(jobs[j]->runTime != jobs[j]->burstTime)
                            jobs[j]->finishTime++;
                    }
                }

                jobs[frontJobIndex]->runTime++;

                if(jobs[frontJobIndex]->runTime == jobs[frontJobIndex]->burstTime)
                    dequeue(intermediateQueue);


                if (intermediateQueue->size != 0) {
                    frontJobIndex = front(intermediateQueue)->jnumber - 1;

                    /* Moves all jobs that are at the intermediate quantum to the background queue */
                    while(jobs[frontJobIndex]->timeInQueue == INTERMEDIATE_QUANTUM) {
                        dequeue(intermediateQueue);
                        jobs[frontJobIndex]->timeInQueue = 0;
                        jobs[frontJobIndex]->queueChange++;
                        enqueue(backgroundQueue, jobs[frontJobIndex]);


                        if (intermediateQueue->size != 0) {
                            frontJobIndex = front(intermediateQueue)->jnumber - 1;
                        }
                    }
                }
            }
            else {
                /* Handles background queue; FCFS */
                if (backgroundQueue->size != 0) {
                    frontJobIndex = front(backgroundQueue)->jnumber - 1;

                    for (int j = 0; j < SIZEOF_ARRAY(jobs); j++) {

                        if(jobs[j]->queueChange == 2) {
                            jobs[j]->timeInQueue++;
                            if(jobs[j]->runTime != jobs[j]->burstTime)
                                jobs[j]->finishTime++;
                        }
                    }

                    jobs[frontJobIndex]->runTime++;
                  

                    if(jobs[frontJobIndex]->runTime == jobs[frontJobIndex]->burstTime) {

                        dequeue(backgroundQueue);

                        if (backgroundQueue->size != 0) {
                            frontJobIndex = front(backgroundQueue)->jnumber - 1;
                        }
                    }
                }
            }
        }
    }


    long sim_end_time = time_usec() / 1000; // end time in ms

    /* adds up all jobs turnaround */
    for (int i = 0; i < SIZEOF_ARRAY(jobs); i++)
    {
        if(jobs[i]->runTime != jobs[i]->burstTime)
            jobs[i]->finishTime = -1;

        jobs[i]->turnaround = jobs[i]->finishTime / jobs[i]->burstTime;
        sim->avg_turnaround = sim->avg_turnaround + jobs[i]->turnaround;
    }

    sim->avg_job_length = sim->avg_job_length / NUMJOBS;
    sim->avg_turnaround = sim->avg_turnaround / NUMJOBS;


    /* Printing Output */

    printf("\n%s\n", "--Input Parameters--");
    printf("%s%d\n", "Seed: ", SEED);
    printf("%s%d\n", "# of Jobs: ", NUMJOBS);
    printf("%s%d%s\n", "Foreground quantum: ", FOREGROUND_QUANTUM, " ms");
    printf("%s%d%s\n", "Intermediate quantum: ", INTERMEDIATE_QUANTUM, " ms");
    printf("%s%d%s\n", "Job Length: ", JOB_LENGTH, " ms");
    printf("%s%d%s\n", "Job Frequency: ", JOB_FREQUENCY, " ms");

    printf("\n%s\n", "--Sim Details--");
    printf("%s%ld%s\n", "Sim Start Time: ", sim_start_time, " ms");
    printf("%s%ld%s\n", "Sim End Time: ", sim_end_time, " ms");
    printf("%s%.2f%s\n", "Average Jog Length: ", sim->avg_job_length, " ms");
    printf("%s%.2f\n", "Average Turnaround Time: ", sim->avg_turnaround);

    if(VERBOSE) {

        printf("\n%s\n", "Job    Arrival Time (ms)    Burst Time (ms)    Queue Change (times)   Finish Time (ms)    Turnaround Time  ");
        printf("%s\n", "===========================================================================================================");

        for(int i=0; i < SIZEOF_ARRAY(jobs); i++) {
            printf("%d\t%d\t\t\t%d\t\t%d\t\t\t%d\t\t\t%d\n", jobs[i]->jnumber, jobs[i]->arrivalTime, jobs[i]->burstTime, jobs[i]->queueChange, jobs[i]->finishTime, jobs[i]->turnaround);
        }
    }

    printf("\n%s\n\n", "--End--");


    return 0;
}

