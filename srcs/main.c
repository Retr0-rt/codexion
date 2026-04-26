#include "codexion.h"

int	main(int ac, char **av)
{
    t_system *system = NULL;
    init_all(system, ac, av);
    int i = 0;
    while(i < 5){
        pthread_join(system->coders[i].thread_id, NULL);
        i++;
    }
    pthread_join(system->monitor_thread, NULL);
}