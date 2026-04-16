#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


void* routine(){
    printf("beginning\n");
    // sleep(3);
    printf("ending\n");
}

int main(int ac, char** av){
    pthread_t t1, t2;

    pthread_create(&t1, NULL, &routine, NULL);
    pthread_create(&t2, NULL, &routine, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}