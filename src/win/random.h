/*
  Purpose: To provide easy access to variable random number
		generation (all psuedo-random)
  This is based off of Sean Eshbaugh's Random Number Generator
  header.  Which was found at 
  http://www.geekpedia.com/tutorial39_Random-Number-Generation.html

  However, his version didn't work unedited so, I get some
  credit (Nate Merlin)
*/
#include <stdlib.h>
#include <sys/time.h>

/* Generate psuedo-random seed vaule.  Use before calling these
   functions */

// Global Variable: Used to check the seed value to make sure it isn't the same
static unsigned testseed;
static unsigned char counter; 

void initrand()
{
 struct timeval tseed;
 unsigned timeseed;

 gettimeofday(&tseed, NULL);
 timeseed = 0 + (int)((tseed.tv_sec) + tseed.tv_usec);

 if (timeseed == testseed) {
	 timeseed += counter;
	 counter++;

 }
 else {
	 testseed = timeseed;
	 counter = 1;
 }
	
 srand(timeseed);
}

/* Generates a psuedo-random integer between min and max */
int randint(int min, int max)
{
	if (min>max)
	{
		return max+(int)((min-max+1)*(rand()/(RAND_MAX+1.0)));
	}
	else
	{
		return min+(int)((max-min+1)*(rand()/(RAND_MAX+1.0)));
	}
}
