//
// Created by Yuechen Liu on 2017-11-01.
//

#ifndef PRJ_STATS_H
#define PRJ_STATS_H

void stats_init(int num_producers);
void stats_cleanup(void);
void stats_record_produced(int factory_number);
void stats_record_consumed(int factory_number, double delay_in_ms);
void stats_display(void);

#endif //PRJ_STATS_H
