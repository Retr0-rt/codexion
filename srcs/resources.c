#include "codexion.h"


static void log_taking_a_dongle(t_coder *coder){
    pthread_mutex_lock(&coder->sys->log_mutex);
    if (!safe_stop_check(coder->sys))
        printf("%lld %d has taken a dongle\n",
            get_relative_time(coder->sys->start_time), coder->id);
    pthread_mutex_unlock(&coder->sys->log_mutex);
}

void take_dongles(t_coder *coder)
{
    if(coder->left_dongle_id < coder->right_dongle_id)
    {
        pthread_mutex_lock(&coder->sys->dongles[coder->left_dongle_id]);
        log_taking_a_dongle(coder);

        pthread_mutex_lock(&coder->sys->dongles[coder->right_dongle_id]);
        log_taking_a_dongle(coder);
    }
    else
    {
        pthread_mutex_lock(&coder->sys->dongles[coder->right_dongle_id]);
        log_taking_a_dongle(coder);

        pthread_mutex_lock(&coder->sys->dongles[coder->left_dongle_id]);
        log_taking_a_dongle(coder);
    }
}

void drop_dongles(t_coder *coder)
{
    pthread_mutex_unlock(&coder->sys->dongles[coder->left_dongle_id]);
    pthread_mutex_unlock(&coder->sys->dongles[coder->right_dongle_id]);
}