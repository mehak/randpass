/*  Copyright 2010 Nathanel Merlin

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
  Author: Nate Merlin
  Purpose: For commonly used functions that I write
  Notes: 
*/
#include <stdio.h>
#include <string.h>
#include <locale.h>

/* Prototypes */
void clrstdin(char*);
int checklocale( void );

/* Checks if the string exited with a newline feed or not.  If it did, it
   doesn't clear stdin.  If it didn't, it reads what is left over from stdin.
*/
void clrstdin(char *string)
{
	char * pch;
	int empty;

/* Searches for a newline character (ascii decimal 10) */
	pch = strchr(string, 10);

	if (!pch) 
		while ((empty = fgetc(stdin)) != '\n' && empty != EOF);
	
}

/* Checks for locale, and sets it.  If it doesn't prints error returns 1 */
int checklocale( void )
{
 if (!setlocale(LC_CTYPE, "")) {
	fprintf(stderr, "Can't set the specified locale! " \
		"Check LANG, LC_CTYPE, LC_ALL. \n");
	return 1;
 }

 return 0;
}
