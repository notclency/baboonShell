#include "types.h"
#include "commands.h"

void signal_handler(int status);

void signal_handler(int status)
{
    wait(&status);
}
