#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#include "command-line-options.h"
#include "ibarland-utils.h"

/**
 * cpusim.c
 * 
 * Project 4 - Process Scheduling
 * Author: Forrest Meade (fmeade)
 * Due: April 19, 2016 @ 2p
 *
 * Description: 
 * 
 * Help: 
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
    ,{ "job-length"          , 'l', "10", "the expected job-length in ms. " } 
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
    bool VERBOSE;

    if (strcmp(settings[6], "true") == 0)
        VERBOSE = true;
    else
        VERBOSE = false;

	int SEED;

    // generates a random number for the seed if not provided
    srandom(time(0));
    if (settings[5] == NULL)
        SEED = random();
        //Now, the seed is a random integer
    else
        SEED = atoi(settings[5]);

    int avg_job_length = 0;
    int avg_job_turnaround = 0;




    printf("\n%s\n", "--Input Parameters--");
    printf("%s%d\n", "Seed: ", SEED);
    printf("%s%d\n", "# of Jobs: ", NUMJOBS);
    printf("%s%d%s\n", "Foreground quantum: ", FOREGROUND_QUANTUM, " ms");
    printf("%s%d%s\n", "Intermediate quantum: ", INTERMEDIATE_QUANTUM, " ms");
    printf("%s%d%s\n", "Job Length: ", JOB_LENGTH, " ms");
    printf("%s%d%s\n", "Job Frequency: ", JOB_FREQUENCY, " ms");

    printf("\n%s\n", "--Job Details--");
    printf("%s%d%s\n", "Average Jog Length: ", avg_job_length, " ms");
    printf("%s%d\n", "Average Turnaround Time: ", avg_job_turnaround);

    if(VERBOSE) {
        for(int i=1; i <= NUMJOBS; i++) {
    	    printf("\n%s%d%s\n", "--Job ", i, " Output--");
    	    printf("%s%d%s\n", "Arrival Time: ", 0, " ms");
    	    printf("%s%s\n", "Queue Change? ", "Hmm");
    	    printf("%s%d%s\n", "Finish Time: ", 0, " ms");
    	    printf("%s%d\n", "Turnaround Time: ", 0);
        }
	}

    printf("\n%s\n\n", "--End--");




	return 0;
}