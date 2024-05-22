#ifndef COMMANDS_H
#define COMMANDS_H

#include "types.h"
#include "lib.h"
#include "signal_handler.h"

int start(allCommands *allCommands);

/*
    description: runs a single command in current process
*/
int runCommand(grouped_command *grouped_command);

/*
    description: runs a 2 stage piped command
*/
int pipedCommands(grouped_command *grouped_command_1, grouped_command *grouped_command_2, int background);

/*
    description: runs a command with input redirected from a file
*/
int inputRedirectedCommands(grouped_command *grouped_command, int fd[2], int background);

/*
    description: runs a command with output redirected to a file
*/
int outputRedirectedCommands(grouped_command *grouped_command, int background);

#endif
