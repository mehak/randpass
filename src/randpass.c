/*  Copyright 2010 Nathanael Merlin

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
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
  Name: randpass
  Author: Nate Merlin
  Purpose: To make secure random passwords
  Notes: 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include "random.h"

/* Version information */
extern const char *version_string;

/* Function prototypes */
char *randpass(const int plength);
int getargs(int argc, char **argv);
int showpass(int passlen, int passnum);
static void usage(FILE *stream, int status);
char genchar(const int charpool);
/* int errorout(int errors); // This needs to be written, used to show errors */

/* Global Variables */
static int plength;         /* Password length */
static int pswnum; 	    /* Number of passwords */
static int seed; 	    /* 0 = use time as the seed, 1 = used provided value */
static const char *outfile; /* Output filename */
static bool append; 	    /* Append to outfile instead of overwriting it */
static FILE *outfp; 	    /* Output stream - default stdout */
static char *program_name;  /* Program that executed this program */
static int charpool;	    /* Replaces the nalpha, nnum, and npunc stuff, check 
			       the randpass function for details */
/*static int errors;	  */  // Used to hold various program specific errors, not implemented 

/* Makes random password of length plength */  
char *randpass(const int plength)
{
 int i; 	 /* i = iteration */
 char *password; /* holds the random password */

 password = (char*) malloc (plength + 1);
 memset (password, 0, plength + 1); /* Set password to all 0's */

 /* Used to generate the password.
  0 - All characters in all positions
  1 - All characters, 1st character is alpha
  2 - Alphanumeeric characters
  3 - Alpha + puncuation 
  4 - numbers & puncuations
  5 - Alpha only
  6 - numbers only
  7 - puncuation only
*/
 for (i = 0; i < plength; i++) {
	 switch(charpool) {
		case 0:
			password[i] = genchar(0);

			break;
		case 1:
			if (i == 0) {
				password[i] = genchar(3);
			}
			else {
				password[i] = genchar(0);
			}

			break;
		case 2:
			password[i] = genchar(1);

			break;
		case 3:
			password[i] = genchar(2);

			break;
		case 4: 
			password[i] = genchar(4);

			break;
		case 5:
			password[i] = genchar(3);

			break;
		case 6:
			password[i] = genchar(5);

			break;
		case 7:
			password[i] = genchar(6);

			break;
		default:
			break;
	}

 }

 return(password);
}

/* This gets command-line arguments. Uses the getopt.h header and system */
int getargs(int argc, char **argv)
{
 int optc;
 static struct option longopts[] =
 {
	{"help",       no_argument,       0, 'h'},
	{"length",     required_argument, 0, 'l'},
	{"number",     required_argument, 0, 'n'},
	{"seed",       required_argument, 0, 's'},
	{"outfile",    required_argument, 0, 'o'},
	{"append",     no_argument,       0, 'a'},
	{"version",    no_argument,	  0, 'V'},
	{"noalpha",    no_argument,       0, 'A'},
	{"nonum",      no_argument,       0, 'N'},
	{"nopunc",     no_argument,       0, 'P'},
	{"nointeract", no_argument,       0, 'R'},
	{0, 0, 0, 0}
 };

/* Initialize variables for use later */
 plength = 8; 		 /* default is password length of 8 characters */
 pswnum = 1; 		 /* default is one password */
 seed = 0; 		 /* default is to use initrand */
 outfile = NULL; 	 /* Output file, only used if -o is used */
 append = false; 	 /* Append to outfile instead of overwriting it */
 outfp = stdout; 	 /* Set outfp to default (stdout) */
 program_name = argv[0]; /* set name of program that called process */
 charpool = 0;		 /* Default is to use all characters */

 while ((optc = getopt_long(argc, argv, "+hVANPRao:l:n:s:", longopts, (int *) 0)) != EOF)
 {
	switch(optc)
	{
	case 'h':
	  usage(stdout, 0);
	case 'V':
	  printf("%s\n", version_string);
          exit(0);
	case 'l':
	  plength = strtol(optarg, NULL, 10);
	  if (plength > 99 || plength < 8) plength = 8;
	  break;
	case 'n':
	  pswnum = strtol(optarg, NULL, 10);
	  if (pswnum > 99999 || pswnum < 1) pswnum = 1;
	  break;
	case 's':
	  seed = strtol(optarg, NULL, 10);
	  if (seed > 0) { 
		srand(seed); 
	  }
	  else {
		fprintf(outfp, "-s requires a positive integer argument:"\
				"using default\n");
	  }
	  break;
	case 'a':
	  append = true;
	  break;
	case 'o':
	  outfile = optarg;
	  break;
	case 'A':
	  charpool += 4;
	  break;
	case 'N':
	  charpool += 3;
	  break;
	case 'P':
	  charpool += 2;
	  break;
	case 'R': // Possible remove, at the very least redefine
	  break;
	default:
	  // Going to implement with errorOut
	  break;
 	}

 }
 
 if (outfile) {
	if (append) 
 	  outfp = fopen(outfile, "a");
	else 
	  outfp = fopen(outfile, "w");
 	if (outfp == NULL) {
	  fprintf(stderr, "Error opening %s \n", outfile);
	  exit(1);
 	}
 }

/* Checking to make sure there are characters in the character pool. */
 if ( charpool == 9 ) {
	 fprintf(stderr, "Error: No characters in character pool: \n" \
			 "Do not use -P, -A, and -N together or " \
			 "--noalpha, -nonum, and --nopunc together.\n");
	 exit(1);
 }

 return 0;
} 

/* here is where the magic happens.  This shows the password or passwords */
int showpass(int passlen, int passnum)
{
 char *password;
 int i;

 for ( i = 0; i < passnum; i++ ) {
	if (seed == 0)
		initrand();
 	password = randpass(passlen);
 	fprintf(outfp, "%s \n", password); // removed "Password:" - Tim H.
	if (outfile) 
		fflush(outfp);
 	free(password);
 }

 if (outfile)
	fclose(outfp);

 return 0;
}

/* This shows the help and usage information */
static void usage(FILE *stream, int status)
{
 fprintf(stream,"\
Usage: %s [OPTIONS] \n\
	  Options may be: \n\
	  [-h] [--help] [-l LENGTH] [-n NUM] [-s SEED] [-o FILE]\n\
	  [--length=LENGTH] [--number=NUM] [--seed=SEED] [--output=FILE]\n\
	  [-a] [--append] [-V] [--version] [-A] [--noalpha] [-N] [--nonum]\n\
	  [-P] [--nopunc] [-R] [--nointeract] \n", program_name);

 exit(status);
}

/* generate characters for the random password.
   0 - All characters, 1 - alphanumeric, 2 - alpha + puncuation
   3 - alpha only, 4 - numbers + puncuation, 5 - numbers only
   6 - puncuation only */
char genchar(const int charpool) 
{
	int rn;

	switch(charpool) {
		case 0:
			rn = randint(33, 126);

			break;
		case 1:
			do {
				rn = randint(48, 122);
			}
			while ( !isalnum(rn) );

			break;
		case 2:
			do {
				rn = randint(33, 126);
			}
			while ( isdigit(rn) );

			break;
		case 3:
			do {
				rn = randint(65, 122);
			}
			while ( !isalpha(rn) );
			break;
		case 4:
			do {
				rn = randint(33, 126);
			}
			while ( isalpha(rn) );

			break;
		case 5:
			rn = randint(48, 57);

			break;
		case 6:
			do {
				rn = randint(33, 126);
			}
			while ( !ispunct(rn) );

			break;
		default:
			// Going to implement with errorout
			break;
	}

	return(rn);
}

/* This is used to show various program specific errors to the user 
   because this program will always display one password of character
   length eight.  Or it will for the most part. */
/*int errorout(int errors)
{ 
}
*/

int main(int argc, char **argv)
{
 getargs(argc, argv);

 showpass(plength, pswnum);

 /* errorout(int errors); */

 return(0);
}
