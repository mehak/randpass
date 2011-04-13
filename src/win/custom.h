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
