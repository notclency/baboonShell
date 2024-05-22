#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <unistd.h>

#include "consts.h"

typedef unsigned char UINT8;
typedef unsigned int UINT16;
typedef unsigned long UINT32;

typedef struct
{
    char *argv[MAX_ARGS]; // the command split into tokens, [0] command, [1] first argument, etc.
    int argc;             // the number of tokens in the command, including the command itself.
} single_command;

typedef struct{
    int piped;             // 1 if the command is piped, 0 otherwise
    int outputRedirected; // 1 if the output is redirected, 0 otherwise
    int inputRedirected;  // 1 if the input is redirected, 0 otherwise

    single_command command_1;
    single_command command_2;
} grouped_command;

typedef struct
{
    int background; // 1 if the command is to be run in the background, 0 otherwise
    int index;      // the index of the command to be run in the command array

    int num_commands; // the number of commands in the command array
    int num_args;     // the number of arguments in the command array

    grouped_command commandArray[MAX_COMMANDS];
} allCommands;

#endif