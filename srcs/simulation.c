#include "codexion.h"


int safe_stop_check(t_system *sys)
{
    int status;
    pthread_mutex_lock(&sys->monitor_mutex);
    status = sys->stop_flag;
    pthread_mutex_unlock(&sys->monitor_mutex);
    return (status);
}

void action_compile(t_coder *coder)
{
    long long relative_time;

    relative_time = get_relative_time(coder->sys->start_time);

    pthread_mutex_lock(&coder->sys->monitor_mutex);
    coder->last_compile_start = relative_time;
    coder->compiles_done++;
    pthread_mutex_unlock(&coder->sys->monitor_mutex);

    pthread_mutex_lock(&coder->sys->log_mutex);
    printf("%lld %d is compiling\n", relative_time, coder->id);
    pthread_mutex_unlock(&coder->sys->log_mutex);
    usleep(coder->sys->time_to_compile * 1000);
}

void action_debug(t_coder *coder){
    long long relative_time;

    relative_time = get_relative_time(coder->sys->start_time);

    pthread_mutex_lock(&coder->sys->log_mutex);
    printf("%lld %d is debugging\n", relative_time, coder->id);
    pthread_mutex_unlock(&coder->sys->log_mutex);
    usleep(coder->sys->time_to_debug * 1000);
}

void action_refactor(t_coder *coder){
    long long relative_time;

    relative_time = get_relative_time(coder->sys->start_time);

    pthread_mutex_lock(&coder->sys->log_mutex);
    printf("%lld %d is refactoring\n", relative_time, coder->id);
    pthread_mutex_unlock(&coder->sys->log_mutex);
    usleep(coder->sys->time_to_refactor * 1000);
}

void *coder_routine(void *arg){

}