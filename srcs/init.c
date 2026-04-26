#include "codexion.h"


int init_system_args(t_system *sys, int ac, char **av){
    sys->nb_coders = atoi(av[0]);
    sys->time_to_burnout = atoi(av[1]);
    sys->time_to_compile = atoi(av[2]);
    sys->time_to_debug = atoi(av[3]);
    sys->time_to_refactor = atoi(av[4]); 
    sys->req_compiles = atoi(av[5]);
    sys->scheduler = av[6];

}

int init_mutexes(t_system *sys)
{
    int i;

    i = 0;
    sys->dongles = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * sys->nb_coders);
    sys->log_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    sys->monitor_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));

    while (i < sys->nb_coders)
    {
        if(pthread_mutex_init(sys->dongles + i, NULL) != 0)
        {
            printf("error initailizing the dongle %d", i + 1);
            return (-1);
        }
        i++;
    }
    if(pthread_mutex_init)
    return (0);
}

int init_coders(t_system *sys)
{
    int i;
    struct timeval currrent_timestamp;
    gettimeofday(&currrent_timestamp, NULL);
    i = 0;
    sys->coders = (t_coder*)malloc(sizeof(t_coder) * sys->nb_coders);

    while (i < sys->nb_coders)
    {

        sys->coders[i].id = i + 1;
        sys->coders[i].last_compile_start = currrent_timestamp.tv_sec * 1000 + currrent_timestamp.tv_usec / 1000;
        sys->coders[i].left_dongle_id = i;
        sys->coders[i].right_dongle_id = (i + 1) % sys->nb_coders;
        sys->coders[i].sys = &sys;
        i++;
    }

}