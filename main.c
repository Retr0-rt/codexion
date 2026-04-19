 #include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_mutex_t mutex_two;

int var2 = 0;
int var = 0;

void *routine(void *arg){
    int *a = (int*)arg;
    if(*a % 2 == 0){
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex_two);
    }
    else{
        pthread_mutex_lock(&mutex_two);
        pthread_mutex_lock(&mutex);
    }
    var+=10;
    var2 += 8;
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex_two);
    return NULL;
}

int main(){
    srand(time(NULL));
    pthread_t th[10];
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_two, NULL);
    for(int i = 0; i < 10; i++){
        int *a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&th[i], NULL, routine, (int*)a))
            perror("failed to create");
    }

    for(int i = 0; i < 10; i++){
        if(pthread_join(th[i], NULL))
            perror("failed to join");
    }
    printf("%d\n", var);
    printf("%d\n", var2);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_two);
    
    return 0;
}