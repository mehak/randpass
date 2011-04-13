/* Copyright 2010 Nathanael Merlin

    This file is part of randpass.

    randpass is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    randpass is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
  Purpose: To provide easy access to variable random number
	   generation (all psuedo-random)
  
  This is based off of Sean Eshbaugh's Random Number Generator
  header.  Which was found at 
  http://www.geekpedia.com/tutorial39_Random-Number-Generation.html

*/
#include <stdlib.h>
#include <sys/time.h>

/* Generate psuedo-random seed vaule.  Use before calling these
   functions */
void initrand()
{
	struct timeval tseed;
	unsigned timeseed;

	gettimeofday(&tseed, NULL);
	timeseed = 0 + (int)((tseed.tv_sec) + tseed.tv_usec);
	
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
