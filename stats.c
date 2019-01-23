//
// Created by Yuechen Liu on 2017-11-01.
//

#include "stats.h"
#include <stdio.h>
#include <stdlib.h>

int num_factory;

struct factory_stat{
    int stats_factory_number;
    int stats_made;
    int stats_eaten;
    double min_delay;
    double avg_delay;
    double max_delay;
};

struct factory_stat* stats_arr;

void stats_init(int num_producers){
    num_factory = num_producers;
    stats_arr= malloc(num_factory* sizeof(struct factory_stat));
    //printf("aaa\n");

    for (int a=0;a<num_factory;a++){

        stats_arr[a].stats_factory_number=a;
        stats_arr[a].stats_made=0;
        stats_arr[a].stats_eaten=0;
        stats_arr[a].max_delay=0;
        stats_arr[a].min_delay=0;
        stats_arr[a].avg_delay=0;
    }
    //printf("bbb\n");
}

void stats_record_produced(int factory_number){
    stats_arr[factory_number].stats_made++;
}

void stats_record_consumed(int factory_number, double delay_in_ms){
    struct factory_stat factory=stats_arr[factory_number];
    factory.stats_eaten++;
    if (delay_in_ms<=factory.min_delay){
        factory.min_delay=delay_in_ms;
    }
    if (delay_in_ms>=factory.max_delay){
        factory.max_delay=delay_in_ms;
    }
    factory.avg_delay = (factory.avg_delay+delay_in_ms) / factory.stats_made;
}

