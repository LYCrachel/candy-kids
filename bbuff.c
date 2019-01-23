//
// Created by Yuechen Liu on 2017-11-01.
//

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "bbuff.h"

pthread_mutex_t mutex;

sem_t* full;
sem_t* empty;

int in, out;

void* candy_arr[BUFFER_SIZE];

void bbuff_init(){
    pthread_mutex_init(&mutex,NULL);
    full=sem_open("/full",O_CREAT, 0, 0);
    empty=sem_open("/empty",O_CREAT, 0, BUFFER_SIZE);
    in=0;
    out=0;
}


void bbuff_blocking_insert(void* item){

    sem_wait(empty);
    pthread_mutex_lock(&mutex);

    // here starts CS
    candy_arr[in]=item;
    in=(in+1)%BUFFER_SIZE;
    printf("inside inserting CS\n");
    // here ends CS

    pthread_mutex_unlock(&mutex);
    printf("unlock inserting mutex \n");
    sem_post(full);
    printf("After inserting in:%d, full:%d, empty:%d\n",in, *full, *empty );
}

void* bbuff_blocking_extract(void){

    sem_wait(full);
    pthread_mutex_lock(&mutex);

    //  mutex=0 -> locked, here starts CS
    void* eaten=candy_arr[out];
    out=(out+1)%BUFFER_SIZE;
    printf("inside extracting CS\n");
    // here ends CS

    pthread_mutex_unlock(&mutex);
    printf("unlock extracting mutex \n");
    sem_post(empty);
    printf("After extracting out:%d, full:%d, empty:%d\n",out, *full, *empty );
    return eaten;


}

_Bool bbuff_is_empty(void){

    if(full==0){

        return true;
    }
    else{

        return false;
    }

}
