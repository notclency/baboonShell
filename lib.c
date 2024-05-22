#include "lib.h"

/*
    description: cleans/initilizes the command array
*/
void cleanCommands(allCommands *commandArray)
{
    commandArray->num_commands = 1;
    commandArray->num_args = 1;
    commandArray->background = FALSE;
    commandArray->index = 0;

    int i = 0;
    while (i < MAX_COMMANDS)
    {
        commandArray->commandArray[i].piped = FALSE;
        commandArray->commandArray[i].outputRedirected = FALSE;
        commandArray->commandArray[i].inputRedirected = FALSE;
        commandArray->commandArray[i].command_1.argc = 0;
        commandArray->commandArray[i].command_2.argc = 0;

        int j = 0;
        while (j < MAX_ARGS)
        {
            commandArray->commandArray[i].command_1.argv[j] = NULL;
            commandArray->commandArray[i].command_2.argv[j] = NULL;
            j++;
        }

        i++;
    }
}

int parseCommandLine(char command[BUFFER_SIZE], allCommands *allCommands)
{
    char *token;

    strtrim(command);
    token = strtok(command, " ");

    int i = 0;
    while (token != NULL && i < MAX_COMMANDS)
    {
        // skip over whitespace in between commands
        while (compareStrings(token, "") == TRUE)
        {
            token = strtok(NULL, " ");
        }

        if (compareStrings(token, "|") == TRUE)
        {
            allCommands->commandArray[i].piped = TRUE;
            allCommands->num_commands++;
            i++;
        }
        else if (compareStrings(token, "<") == TRUE)
        {
            allCommands->commandArray[i].inputRedirected = TRUE;
            token = strtok(NULL, " "); // get the file name

            // add the file name to the command_2 argv array
            allCommands->commandArray[i].command_2.argv[allCommands->commandArray[i].command_2.argc++] = token;

            // increment the number of arguments
            allCommands->num_args++;
        }
        else if (compareStrings(token, ">") == TRUE)
        {
            allCommands->commandArray[i].outputRedirected = TRUE;
            token = strtok(NULL, " "); // get the file name

            // add the file name to the command_2 argv array
            allCommands->commandArray[i].command_2.argv[allCommands->commandArray[i].command_2.argc++] = token;

            // increment the number of arguments
            allCommands->num_args++;
        }
        else if (compareStrings(token, "&") == TRUE)
        {
            allCommands->background = TRUE;
            token = strtok(NULL, " "); // get next token
            if (token != NULL)
            {
                write(1, "\e[0;31m", 7);
                write(1, "Error: & must be at the end of the command.\n", 44);
                write(1, "\e[0m", 4);
                return -1;
            }
        }
        else
        {
            int index = allCommands->commandArray[i].command_1.argc;
            allCommands->commandArray[i].command_1.argv[index] = token;
            allCommands->commandArray[i].command_1.argc++;
            allCommands->num_args++;
        }

        token = strtok(NULL, " ");
    }

    return 0;
}

char *strtok(char *str, const char *delim)
{
    static char *next = NULL;
    char *ret = NULL;

    if (str != NULL)
    {
        next = str;
    }

    if (next == NULL)
    {
        return NULL;
    }

    ret = next;

    while (*next != '\0')
    {
        if (*next == *delim)
        {
            *next = '\0';
            next++;
            return ret;
        }
        next++;
    }

    next = NULL;

    return ret;
}

int compareStrings(char *str1, char *str2)
{
    int i = 0;

    while (str1[i] != '\0' && str2[i] != '\0')
    {
        if (str1[i] != str2[i])
        {
            return FALSE;
        }
        i++;
    }

    if (str1[i] != '\0' || str2[i] != '\0')
    {
        return FALSE;
    }

    return TRUE;
}

void strtrim(char *s)
{
    int count = 0;
    while (s[count] == ' ' || s[count] == '\n' || s[count] == '\t')
        count++;

    // if no whitespace characters were found, we don't need to do anything!
    if (count != 0)
    {
        // shift the remainder of the string contents up by the number of leading
        // white space characters found
        int i = 0;
        while (s[i + count] != '\0')
        {
            s[i] = s[i + count];
            i++;
        }

        // terminate the string earlier given that the contents are being reduced,
        // the null terminator is shifted up by the number of leading whitespace
        // characters as well
        s[i] = '\0';
    }

    // ------------------ trim whitespaces at the back --------------------------------

    int i = strlength(s) - 1;

    // unless we've reached the end of the string, keep examining characters
    while (i >= 0)
    {
        // if we ecounter a whitespace character, keep checking backwards through
        // the string, otherwise break as we have found the end of the trailing
        // whitespace characters
        if (s[i] == ' ' || s[i] == '\n' ||
            s[i] == '\t')
            i--;
        else
            break;
    }

    // set the null terminator where the final whitespace character occurs to
    // end the string here
    s[i + 1] = '\0';
}

int strlength(char *str)
{
    int i = 0;

    while (str[i] != '\0')
    {
        i++;
    }

    return i;
}

void prompt()
{
    char ccwd[BUFFER_SIZE] = "admin@baboon:~";
    char cwd[BUFFER_SIZE];
    getcwd(cwd, BUFFER_SIZE);

    write(1, "\033[0;32m", 8);
    write(1, ccwd, strlength(ccwd));

    write(1, "\e[0;34m", 8);
    write(1, cwd, strlength(cwd));

    write(1, "\033[0m", 4);
    write(1, "$ ", 2);
}