#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

void *roll_dice(){
    int value = (rand() % 19) + 1;
    int *res = malloc(sizeof(int));

    *res = value;
    return (int*)res;
}

int main(){
    pthread_t t1;
    int *res;

    srand(time(NULL));
    pthread_create(&t1, NULL, &roll_dice, NULL);
    pthread_join(t1, (void **)&res);

    printf("result: %d\n", *res);
    return 0;
}