#include "commands.h"
#include "signal_handler.h"


int start(allCommands *allCommands)
{

    int i = 0;

    while (i < allCommands->num_commands)
    {
        if (allCommands->commandArray[i].piped == TRUE)
        {
            pipedCommands(&allCommands->commandArray[i], &allCommands->commandArray[i + 1], allCommands->background);
            return 0;
        }
        else
        {
            if (allCommands->commandArray[i].inputRedirected == TRUE)
            {
                inputRedirectedCommands(&allCommands->commandArray[i], NULL, allCommands->background);
            }
            else if (allCommands->commandArray[i].outputRedirected == TRUE)
            {
                outputRedirectedCommands(&allCommands->commandArray[i], allCommands->background);
            }
            else
            {
                // check if the command is cd for changing directories
                if (compareStrings(allCommands->commandArray[i].command_1.argv[0], "cd") == TRUE)
                {
                    chdir(allCommands->commandArray[i].command_1.argv[1]);
                    return 0;
                }

                int pid = fork();

                if (pid == -1)
                {
                    write(1, "\e[0;31m", 7);
                    write(1, "Forking Error - 1", 13);
                    write(1, "\e[0m", 4);
                    return -1;
                }
                else
                {
                    if (pid == 0)
                    {
                        signal(SIGINT, signal_handler);
                        signal(SIGCHLD, signal_handler);
                        runCommand(&allCommands->commandArray[i]);
                        _exit(0);
                    }

                    waitpid(pid, NULL, 0);
                }
            }
        }

        i++;
    }

    return 0;
}

int pipedCommands(grouped_command *grouped_command_1, grouped_command *grouped_command_2, int background)
{

    int pipefd[2];
    pipe(pipefd);

    int pid_1, pid_2;
    int child_status;

    pid_1 = fork();

    if (pid_1 == -1)
    {
        write(1, "\e[0;31m", 7);
        write(1, "Forking Error - 1", 13);
        write(1, "\e[0m", 4);
        return -1;
    }
    else
    {
        if (pid_1 == 0)
        {
            if (grouped_command_1->inputRedirected == TRUE)
            {
                inputRedirectedCommands(grouped_command_1, pipefd, background);
            }
            else if (grouped_command_1->outputRedirected == TRUE)
            {
                outputRedirectedCommands(grouped_command_1, background);
            }
            else
            {
                signal(SIGINT, signal_handler);
                signal(SIGCHLD, signal_handler);
                close(pipefd[READ_END]);
                dup2(pipefd[WRITE_END], STDOUT_FILENO);

                char commandPath[20] = "/usr/bin/";
                strcat(commandPath, grouped_command_1->command_1.argv[0]);

                runCommand(grouped_command_1);
            }

            _exit(0);
        }
    }

    close(pipefd[WRITE_END]);

    pid_2 = fork();

    if (pid_2 == -1)
    {
        write(1, "\e[0;31m", 7);
        write(1, "Forking Error - 2", 13);
        write(1, "\e[0m", 4);
        return -1;
    }
    else
    {
        if (pid_2 == 0)
        {
            signal(SIGINT, signal_handler);
            signal(SIGCHLD, signal_handler);
            close(pipefd[WRITE_END]);
            dup2(pipefd[READ_END], STDIN_FILENO);

            char commandPath[20] = "/usr/bin/";
            strcat(commandPath, grouped_command_2->command_1.argv[0]);

            runCommand(grouped_command_2);

            _exit(0);
        }

        close(pipefd[READ_END]);
    }

    if(background == FALSE){
        waitpid(pid_1, &child_status, 0);
        waitpid(pid_2, &child_status, 0);
    }

    return 0;
}

int runCommand(grouped_command *grouped_command)
{
    char commandPath[20] = "/usr/bin/";
    strcat(commandPath, grouped_command->command_1.argv[0]);

    int err = execve(commandPath, grouped_command->command_1.argv, NULL);

    if (err == -1)
    {
        write(1, "\e[0;31m", 7);
        write(1, "Error executing command.\n", 25);
        write(1, "\e[0m", 4);
    }

    return err;
}

int inputRedirectedCommands(grouped_command *grouped_command, int pipefd[2], int background)
{

    int pid_1;

    pid_1 = fork();

    if (pid_1 == -1)
    {
        write(1, "\e[0;31m", 7);
        write(1, "Forking Error - 1", 13);
        write(1, "\e[0m", 4);
        return -1;
    }
    else
    {
        if (pid_1 == 0)
        {
            signal(SIGINT, signal_handler);
            signal(SIGCHLD, signal_handler);
            int fd = open(grouped_command->command_2.argv[0], O_RDONLY, 0777);
            dup2(fd, STDIN_FILENO);
            close(fd);

            // wrirte to the write end of fd if the command is piped
            if (grouped_command->piped == TRUE)
            {
                close(pipefd[READ_END]);
                dup2(pipefd[WRITE_END], STDOUT_FILENO);
            }

            char commandPath[20] = "/usr/bin/";
            strcat(commandPath, grouped_command->command_1.argv[0]);

            int err = execve(commandPath, grouped_command->command_1.argv, NULL);

            if (err == -1)
            {
                write(1, "Error executing command.\n", 25);
            }

            exit(EXIT_SUCCESS);
        }

        if(background == FALSE){
            waitpid(pid_1, NULL, 0);
        }
    }

    return 0;
}

// /*
//  * This function is called when the command is output redirected.
//  * It will run the first command, and redirect its output to the file name path
//  * specified in the second command. If the file does not exist, it will be created.
//  */
int outputRedirectedCommands(grouped_command *grouped_command, int background)
{

    int pid_1;

    pid_1 = fork();

    if (pid_1 == -1)
    {
        write(1, "Forking Error - 1", 13);
        return -1;
    }
    else
    {
        if (pid_1 == 0)
        {
            signal(SIGINT, signal_handler);
            signal(SIGCHLD, signal_handler);
            remove(grouped_command->command_2.argv[0]);
            int fd = open(grouped_command->command_2.argv[0], O_CREAT | O_WRONLY, 0777);
            dup2(fd, STDOUT_FILENO);
            close(fd);

            char commandPath[20] = "/usr/bin/";
            strcat(commandPath, grouped_command->command_1.argv[0]);

            int err = execve(commandPath, grouped_command->command_1.argv, NULL);

            if (err == -1)
            {
                write(1, "Error executing command.\n", 25);
            }

            exit(EXIT_SUCCESS);
        }

        if (background == FALSE)
        {
            waitpid(pid_1, NULL, 0);
        }
    }

    return 0;
}