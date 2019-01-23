#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>

#include "bbuff.h"
#include "stats.h"

//global variables
int num_factories;
int num_kids;
int num_sec;
_Bool stop_thread = false;


struct candy_t{
    int factory_number;
    double time_stamp_in_ms;
};


void* factory_produce(void* arg);
void* kids_consume(void* arg);
double current_time_in_ms(void);









int main(int argc, char** argv)
{

    //printf("aaa\n");
// 1. Extract arguments
    if (argc < 4) {
        printf("Usage: %s <num 1> <num 2> <num-3>\n", argv[0]);
        exit(-1);
    }
    for (int i=1;i<=3;i++){
        if(!isdigit(*argv[i]) || atoi(argv[i])<0) {
            printf(" Need command line parameter to be positive integer.\n");
            return 1;
        }
    }



// 2. Initialize modules
    //initialize arrays
    num_factories=atoi(argv[1]);
    num_kids=atoi(argv[2]);
    num_sec=atoi(argv[3]);

    bbuff_init();
    stats_init(num_factories);



// 3. Launch candy-factory threads
    int factory_ref[num_factories];
    for (int x=0;x< num_factories; x++){
        factory_ref[x]=x;
    }
    pthread_t f_ids[num_factories];
    for (int i = 0; i < num_factories; i++) {
        pthread_create(&f_ids[i], NULL, factory_produce, &factory_ref[i]);
    }



// 4. Launch kid threads
    pthread_t k_ids[num_kids];
    for (int m = 0; m < num_kids; m++) {
        pthread_create(&k_ids[m], NULL, kids_consume, NULL);
    }



// 5. Wait for requested time
    for (int j = 0; j<num_sec; j++){
        sleep(1);
        printf("Time %d s\n", j);
    }



// 6. Stop candy-factory threads
    printf("Stop candy-factory threads.\n");
    stop_thread=true;
    for (int i = 0; i < num_factories; i++) {
        pthread_join(f_ids[i],NULL);
    }



// 7. Wait until no more candy
    printf("Wait until no more candy.\n");
    while (!bbuff_is_empty()){
        printf("Waiting for all candy to be consumed.\n");
        sleep(1);
    }



// 8. Stop kid threads
    printf("Stop kid threads.\n");
    for (int n = 0; n < num_kids; n++) {
        pthread_cancel(k_ids[n]);
        pthread_join(k_ids[n], NULL);
        printf("Have stopped kid # %d\n",n);
    }



// 9. Print statistics
// 10. Cleanup any allocated memory


    return 0;
}













void* factory_produce(void* arg){
    int* facory_number_ptr= (int*)arg;
    int factory_number=*facory_number_ptr;

    while (!stop_thread){
        int sleep_factory=rand()%3+1;
        printf("\tFactory %d ships candy and waits(sleeps) %d s \n",factory_number,sleep_factory);

        struct candy_t* candy=malloc(sizeof(struct candy_t)); //count candies
        candy->factory_number=factory_number;
        candy->time_stamp_in_ms=current_time_in_ms();
        printf("ccc\n");
        bbuff_blocking_insert(candy);
        stats_record_produced(factory_number);
        sleep(sleep_factory);
    }

    printf("Factory %d is Done!\n", factory_number);
    pthread_exit(0);
}

void* kids_consume(void* arg){
    while (true){
        struct candy_t* eaten_candy= (struct candy_t*)bbuff_blocking_extract();
        double current_time = current_time_in_ms();
        stats_record_consumed(eaten_candy->factory_number, (current_time - eaten_candy->time_stamp_in_ms));
        //free(eaten_candy);
        int sleep_kid=rand()%2;
        printf("Kids consuming...and about to sleep for %d seconds\n", sleep_kid);
        sleep(sleep_kid);

    }
}

double current_time_in_ms(void) {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}
