#include "types.h"
#include "commands.h"

/*
    description: gets a command from the user, stores it in a Command struct, returns the Command struct
*/
void getCommandLine(char command[BUFFER_SIZE]);

int main()
{
    /*
        main loop
    */

    // command struct
    char commandLine[BUFFER_SIZE];
    getCommandLine(commandLine);

    // array of command arrays
    allCommands allCommands;
    cleanCommands(&allCommands);

    int err = parseCommandLine(commandLine, &allCommands);

    while (compareStrings(allCommands.commandArray[0].command_1.argv[0], "exit") == FALSE)
    {
        if (err != -1)
        {
            if (allCommands.background == TRUE)
            {
                write(1, "Background Process Running ...\n", 31);
            }
            start(&allCommands);
        }

        cleanCommands(&allCommands);

        getCommandLine(commandLine);
        err = parseCommandLine(commandLine, &allCommands);
    }
}

void getCommandLine(char commandLine[BUFFER_SIZE])
{

    int bytes_read = 0;

    prompt();
    bytes_read = read(1, commandLine, BUFFER_SIZE);

    // if the user just presses enter, read again
    while (bytes_read == 1) // 1 == '\n'
    {
        prompt();
        bytes_read = read(1, commandLine, BUFFER_SIZE);
    }

    commandLine[bytes_read - 1] = '\0';
}

