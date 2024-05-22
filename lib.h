#ifndef LIB_H
#define LIB_H

#include "types.h"

/*
    description: parses a command, stores all the commands in the command array.
*/
int parseCommandLine(char command[BUFFER_SIZE], allCommands *commandArray);

/*
    description: splits a string into tokens, returns a pointer to the first token
*/
char *strtok(char *str, const char *delim);

/*
    description: compares two strings, returns TRUE if they are equal, FALSE otherwise
*/
int compareStrings(char *str1, char *str2);

/*
    description: trims leading and trailing whitespace from a string
*/
void strtrim (char *str);

/*
    description: returns the length of a string
*/
int strlength(char *str);

/*
    description: resets the command array
*/
void cleanCommands(allCommands *commandArray);

/*
    description: prints the prompt
*/
void prompt();


#endif