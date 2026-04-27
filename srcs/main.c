#include "codexion.h"

int main(int ac, char **av)
{
    t_system sys;
    // Codexion requires exactly 8 arguments after the program name
    // ./codexion [coders] [burnout] [compile] [debug] [refactor] [req_compiles] [cooldown] [scheduler]
    if (ac != 9)
    {
        printf("Error: Invalid number of arguments.\n");
        return (1);
    }

    // Pass av + 1 to skip the "./codexion" string
    if (init_all(&sys, ac, av + 1) != 0)
    {
        printf("Error: Initialization failed.\n");
        return (1);
    }

    // The main thread waits here until the simulation finishes
    cleanup_system(&sys);

    return (0);
}