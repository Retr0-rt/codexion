#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;
int var = 0;
void *routine(){
    sleep(1);
    printf("test\n");
}

int main(){
    pthread_t th[10];

    for(int i = 0; i < 10; i++){
        if(pthread_create(&th[i], NULL, routine, NULL))
            perror("faield to create");
        pthread_detach(th[i]);
    }

    for(int i = 0; i < 10; i++){
        if(pthread_join(th[i], NULL))
            perror("faield to join");
    }
}