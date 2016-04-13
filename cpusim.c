#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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
  {  { "numJobs"             , 'n', "5", "the number of jobs to complete, before ending the simulation. " }
    ,{ "foreground-quantum"  , 'f', "100", "the time (in ms) jobs in the foreground queue get, 
    										before being moved to the intermediate queue. " } 
    ,{ "intermediate-quantum", 'i', "200", "the time (in ms) jobs in the intermediate queue get, 
    										before being moved to the background queue. " } 
    ,{ "job-length"          , 'l', "300", "the average job-length in ms. " } 
    ,{ "job-frequency"       , 'r', "1", "the average number of new jobs per ms. " } 
    ,{ "seed"                , 's', "1", "The random-number seed for the simulation. " } 
    ,{ "verbose"             , 'v', "0", "enable verbose output or not. " } 
  };

int main(int argc, char** argv) {

	char** settings = allOptions( argc, argv, NUM_OPTIONS, options );
    // Now, the array `settings` contains all the options, in order:
    // either taken from the command-line, or from the default given in `options[]`.


	return 0;
}