#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>


int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void *routine(void *arg){
    int index = *(int*)arg;
    printf("%d ", primes[index]);
    free(arg);
}

int main(){
    pthread_t th[10];

    for (int i = 0; i < 10; i++){
        int *a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&th[i], NULL, &routine, a)){
            perror("create failed");
        }
    }

    for(int i = 0; i < 10; i++){
        if(pthread_join(th[i], NULL)){
            perror("join failed");
        }
    }
    printf("\n");
}