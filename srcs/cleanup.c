#include "codexion.h"

void cleanup_system(t_system *sys)
{
    int i;

    // 1. Wake any coders stuck waiting on dongle condvars
    i = 0;
    while (i < sys->nb_coders)
    {
        pthread_mutex_lock(&sys->dongles[i].mutex);
        pthread_cond_broadcast(&sys->dongles[i].cond);
        pthread_mutex_unlock(&sys->dongles[i].mutex);
        i++;
    }

    // 2. Wait for all coder threads to finish
    i = 0;
    while (i < sys->nb_coders)
    {
        pthread_join(sys->coders[i].thread_id, NULL);
        i++;
    }

    // 3. Wait for the monitor thread to finish
    pthread_join(sys->monitor_thread, NULL);

    // 4. Destroy all mutexes and condition variables
    i = 0;
    while (i < sys->nb_coders)
    {
        pthread_mutex_destroy(&sys->dongles[i].mutex);
        pthread_cond_destroy(&sys->dongles[i].cond);
        free(sys->dongles[i].queue->array);
        free(sys->dongles[i].queue);
        pthread_mutex_destroy(&sys->coders[i].coder_mutex);
        i++;
    }
    pthread_mutex_destroy(&sys->state_mutex);
    pthread_mutex_destroy(&sys->log_mutex);
    pthread_cond_destroy(&sys->start_gun_cv);

    // 5. Free the allocated arrays
    free(sys->dongles);
    free(sys->coders);
}