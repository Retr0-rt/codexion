#include "codexion.h"

void cleanup_system(t_system *sys)
{
    int i;

    // 1. Wait for all coder threads to finish
    i = 0;
    while (i < sys->nb_coders)
    {
        pthread_join(sys->coders[i].thread_id, NULL);
        i++;
    }

    // 2. Wait for the monitor thread to finish
    pthread_join(sys->monitor_thread, NULL);

    // 3. Destroy all mutexes
    i = 0;
    while (i < sys->nb_coders)
    {
        pthread_mutex_destroy(&sys->dongles[i]);
        pthread_mutex_destroy(&sys->coders[i].coder_mutex);
        i++;
    }
    pthread_mutex_destroy(&sys->state_mutex);
    pthread_mutex_destroy(&sys->log_mutex);
    
    // 4. Free the allocated arrays
    free(sys->dongles);
    free(sys->coders);
}