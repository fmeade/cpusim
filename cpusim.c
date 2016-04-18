#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

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
 * Description: 
 * 
 * Help: Queues - http://www.thelearningpoint.net/computer-science/data-structures-queues--with-c-program-source-code
 * 
 */

 /* The possible command-line options to the program. 
 */
struct option_info options[] =
  {  { "numJobs"             , 'n', "4", "the number of jobs to complete, before ending the simulation. " }
    ,{ "foreground-quantum"  , 'f', "5", "the time (in ms) jobs in the foreground queue get, " 
    										"before being moved to the intermediate queue. " } 
    ,{ "intermediate-quantum", 'i', "10", "the time (in ms) jobs in the intermediate queue get, " 
    										"before being moved to the background queue. " } 
    ,{ "job-length"          , 'l', "4", "the expected job-length in ms. " } 
    ,{ "job-frequency"       , 'r', "1", "the expected number of new jobs per ms. " } 
    ,{ "seed"                , 's', NULL, "The random-number seed for the simulation. " } 
    ,{ "verbose"             , 'v', "true", "enable verbose output or not. " } 
    ,{ "numQueues"           , 'q', "3", "Number of queues for processing to use. "}
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
    const int NUMQUEUES = atoi(settings[7]);


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
    for(int i = 0; i < SIZEOF_ARRAY(jobs); i++) {
        jobs[i] = createJob(i+1, (int)random() % NUMJOBS*JOB_LENGTH, (int)random() % (NUMJOBS*JOB_LENGTH / 2));
        sim->avg_job_length = sim->avg_job_length + jobs[i]->burstTime;
    }


    Queue* queues[NUMQUEUES];
    /* Creates given number of Queues to use starting with highest priority first */
    for (int i = 0; i < SIZEOF_ARRAY(queues); i++) {
        queues[i] = createQueue(NUMJOBS);
    }



    /* Simulated stuff */


    long sim_end_time = time_usec() / 1000; // end time in ms

    sim->avg_job_length = sim->avg_job_length / NUMJOBS;



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
    printf("%s%f%s\n", "Average Jog Length: ", sim->avg_job_length, " ms");
    printf("%s%f\n", "Average Turnaround Time: ", sim->avg_turnaround);

    if(VERBOSE) {
        for(int i=0; i < SIZEOF_ARRAY(jobs); i++) {
    	    printf("\n%s%d%s\n", "--Job ", jobs[i]->jnumber, " Output--");
    	    printf("%s%d%s\n", "Arrival Time: ", jobs[i]->arrivalTime, " ms");
    	    printf("%s%d\n", "Queue Change: ", jobs[i]->queueChange);
    	    printf("%s%d%s\n", "Finish Time: ", jobs[i]->finishTime, " ms");
    	    printf("%s%d\n", "Turnaround Time: ", jobs[i]->turnaround);
        }
	}

    printf("\n%s\n\n", "--End--");




	return 0;
}