#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H
#include <sys/signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>

// Signal handler for SIGINT, SIGTERM, SIGQUIT,SIGCHLD

void signal_handler(int status);



#endif