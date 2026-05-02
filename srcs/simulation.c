#include "codexion.h"


int safe_stop_check(t_system *sys)
{
    int status;
    pthread_mutex_lock(&sys->state_mutex);
    status = sys->stop_flag;
    pthread_mutex_unlock(&sys->state_mutex);
    return (status);
}

void action_compile(t_coder *coder)
{
    long long relative_time;

    relative_time = get_relative_time(coder->sys->start_time);

    pthread_mutex_lock(&coder->coder_mutex);
    coder->last_compile_start = relative_time;
    coder->compiles_done++;
    pthread_mutex_unlock(&coder->coder_mutex);

    pthread_mutex_lock(&coder->sys->log_mutex);
    // no coder can print any logs after someone burnout
    if(!safe_stop_check(coder->sys))
        printf("%lld %d is compiling\n", relative_time, coder->id);
    pthread_mutex_unlock(&coder->sys->log_mutex);
    ft_msleep(coder->sys->time_to_compile, coder->sys);
}

void action_debug(t_coder *coder)
{
    long long relative_time;

    relative_time = get_relative_time(coder->sys->start_time);

    pthread_mutex_lock(&coder->sys->log_mutex);
    if(!safe_stop_check(coder->sys))
        printf("%lld %d is debugging\n", relative_time, coder->id);
    pthread_mutex_unlock(&coder->sys->log_mutex);
    ft_msleep(coder->sys->time_to_debug, coder->sys);
}

void action_refactor(t_coder *coder)
{
    long long relative_time;

    relative_time = get_relative_time(coder->sys->start_time);

    pthread_mutex_lock(&coder->sys->log_mutex);
    if(!safe_stop_check(coder->sys))
        printf("%lld %d is refactoring\n", relative_time, coder->id);
    pthread_mutex_unlock(&coder->sys->log_mutex);
    ft_msleep(coder->sys->time_to_refactor, coder->sys);
}

void *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    wait_for_start(coder->sys);
    if (coder->id % 2 != 0)
        usleep(500);
    while (!safe_stop_check(coder->sys))
    {
        take_dongles(coder);
        action_compile(coder);
        drop_dongles(coder);
        action_debug(coder);
        action_refactor(coder);
    }
    return (NULL);
}
