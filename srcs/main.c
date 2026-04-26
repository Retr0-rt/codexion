#include "codexion.h"

void *routine1(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    for (size_t i = 0; i < 5000000000; i++){
        i++;
    }
    long long relative_time = get_relative_time(start);
    printf("%ldms\n", relative_time);
}

void *routine2(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    ft_msleep(10000);
    long long relative_time = get_relative_time(start);
    printf("%ldms\n", relative_time);
}
int	main(int ac, char **av)
{
   
    t_system *system;
    pthread_t th[2];
    pthread_create(&th[0], NULL, &routine1, NULL);
    pthread_create(&th[1], NULL, &routine2, NULL);
    pthread_join(th[0], NULL);
    pthread_join(th[1], NULL);
   
    
   

    // init_all(system, ac, av);
    // printf("%lld", system->start_time);
}