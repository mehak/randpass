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
#include "custom.h"

/* Version information */
extern const char *version_string;

/* Function prototypes */
char *randpass(const int plength);
int getargs(int argc, char **argv);
int getplength( void );
int getpnum( void );
int removeoptions( void );
int showpass(int passlen, int passnum);
static void usage(FILE *stream, int status);

/* Global Variables */
static int plength; /* Password length */
static int pswnum; /* Number of passwords */
static int seed; /* 0 = use time as the seed, 1 = used provided value */
static const char *outfile; /* Output filename */
static bool append; /* Append to outfile instead of overwriting it */
static FILE *outfp; /* Output stream - default stdout */
static char *program_name; /* Program that executed this program */
static bool nalpha; /* remove alphabet characters from character pool */
static bool nnum;   /* remove numeric characters from character pool */
static bool npunc;  /* remove punctuation marks from character pool */
static bool rinteract; /* removes interactive bits of the program.
			  Only non-vital bits.  We still need at least a 
			  password length and number of passwords */

/* Makes random password of length plength */  
char *randpass(const int plength)
{
 int i, rn; /* i = iteration, rn = random number */
 char *password; /* holds the random password */

 password = (char*) malloc (plength + 1);
 memset (password, 0, plength + 1); /* Set password to all 0's */
 
 for (i = 0; i < plength; i++) {
	if ( (i == 0 && !nalpha) || (nnum && npunc) ) {
		do {
			rn = randint(65,122);
		}
		while (!isalpha(rn));
		password[i] = rn;
	}
	else if (npunc && !nalpha && !nnum) {
		do {
			rn = randint(48,122);
		}
		while (!isalnum(rn));
		password[i] = rn;
	}
	else if (nalpha && npunc) {
		rn = randint(48,57);
		password[i] = rn;
	}
	else if (nalpha && nnum) {
		do {
			rn = randint(33,126);
		}
		while (!ispunct(rn));
		password[i] = rn;
	}
	else if (nalpha && !nnum && !npunc) {
		do {
			rn = randint(33, 126);
		}
		while(isalpha(rn));
		password[i] = rn;
	}
	else {
		rn =  randint(33,126);
		password[i] = rn;
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
 plength = 0; /* default is to interactively get the length of your password */
 pswnum = 0; /* default is to interactively get the number of passwords */
 seed = 0; /* default is to use initrand */
 outfile = NULL; /* Output file, only used if -o is used */
 append = false; /* Append to outfile instead of overwriting it */
 outfp = stdout; /* Set outfp to default (stdout) */
 program_name = argv[0]; /* set name of program that called process */
 nalpha = false; /* Default is to use alphabet characters */
 nnum = false;   /* Default is to use number characters */
 npunc = false;  /* Default is to use punctuation characters */
 rinteract = false; /* Default is to interact with the user */

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
	  if (plength > 99 || plength < 8) plength = 0;
	  break;
	case 'n':
	  pswnum = strtol(optarg, NULL, 10);
	  if (pswnum > 99999 || pswnum < 1) plength = 0;
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
	  nalpha = true;
	  break;
	case 'N':
	  nnum = true;
	  break;
	case 'P':
	  npunc = true;
	  break;
	case 'R':
	  rinteract = true;
	  break;
	default:
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

/* Checking to make sure there are characters in the character pool */
 if ( nnum && nalpha && npunc ) {
	 fprintf(stderr, "Error: No characters in character pool: \n" \
			 "Do not use -P, -A, and -N together or " \
			 "--noalpha, -nonum, and --nopunc together.\n");
	 exit(1);
 }
 

 return 0;
} 

/* This is used to get and validate plength which holds the length of the
 * password */
int getplength( void )
{
 char pstrlen[3];
 int plength;

 do {
	printf("Please provide a password length between 8 and 99:\n");
	fgets(pstrlen, 3, stdin);
	plength = strtol(pstrlen, NULL, 10);
	clrstdin(pstrlen);
 }
 while (plength < 8);

 return(plength);
}

/* This gets the amount of passwords that the user wants and validates user
 * input */
int getpnum( void )
{
 char numstrlen[6];
 int pnum;

 do {
	printf("How many passwords would you like?(up to 99999)\n");
	fgets(numstrlen, 6, stdin);
	pnum = strtol(numstrlen, NULL, 10);
	clrstdin(numstrlen);
 }
 while (pnum < 1);

 return(pnum);
}

/* Determines if the user would like to remove characters from the password
 * character set */
int removeoptions( void )
{
 static bool opttest;
 static char option;

 opttest = false;

 do {
	printf("To remove certain characters from being used in the passwords"\
	       " please type the corresponding letter.  To exit press E then"\
	       " enter.  Possible removal options: \n" \
	       "A - Remove alphabet characters \n" \
	       "N - Remove numeric characters \n" \
	       "P - Remove punctuation characters \n" \
	       "Press enter after pressing a character \n" \
	       "Case is not important. \n"
	       "Setting all options resets them to their default values. \n\n");

	if (nnum && nalpha && npunc) {
		printf("\n\nRESETTING CHARACTER FLAGS. \n\n\n");
		nalpha = false;
		nnum = false;
		npunc = false;
	}
	
	if (nalpha) printf("Alphabet characters will not be used \n");
	if (nnum)   printf("Numbers will not be used \n");
	if (npunc)  printf("Puncuation marks will not be used \n");

	option = toupper(fgetc(stdin));
	clrstdin(&option);

	switch(option) {
	  case 'E':
		opttest = true;
		break;
	  case 'A':
		nalpha = true;
		break;
	  case 'N':
		nnum = true;
		break;
	  case 'P':
		npunc = true;
		break;
	  default:
		break;
	}
 }
 while ( opttest == false );

 return(0);
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

int main(int argc, char **argv)
{
 getargs(argc, argv);

 if (plength == 0)
 	plength = getplength();

 if (pswnum == 0)
 	pswnum = getpnum();

 if (!nnum && !nalpha && !npunc && !rinteract)
	 removeoptions();
 
 showpass(plength, pswnum);

 return(0);
}
