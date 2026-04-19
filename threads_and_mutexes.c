#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

int var = 0;
pthread_mutex_t mutex;


void* routine(){
    int local_sum = 0; 

    // 2. Do the heavy lifting in parallel without any locks!
    for(int i = 0; i < 10000000; i++){
        local_sum++;
    }

    // 3. Lock the mutex ONLY ONCE to safely update the shared global variable
    pthread_mutex_lock(&mutex);
    var += local_sum;
    pthread_mutex_unlock(&mutex);
    
    return NULL;
    // for(int i = 0; i < 10000000; i++){
    //     pthread_mutex_lock(&mutex);
    //     var++;
    //     pthread_mutex_unlock(&mutex);
    // }
}

int main(int ac, char** av){
    struct timeval start, end;
    pthread_t th[8];
    pthread_mutex_init(&mutex, NULL);

    gettimeofday(&start, NULL);


    for(int i = 0; i < 8; i++){
        routine();
    }
    // for (int i = 0; i < 8; i++){
    //     pthread_create(&th[i], NULL, &routine, NULL);
    // }
    // // pthread_create(&t2, NULL, &routine, NULL);
    // // pthread_join(t2, NULL); 
    
    // for (int i = 0; i < 8; i++){
    //     pthread_join(th[i], NULL);
    // }



    gettimeofday(&end, NULL);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("execution time: %fs\n", time_spent);
    printf("number: %d\n", var);
    pthread_mutex_destroy(&mutex);
    return 0;
}