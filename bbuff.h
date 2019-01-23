//
// Created by Yuechen Liu on 2017-11-01.
//

#ifndef PRJ_BBUFF_H
#define PRJ_BBUFF_H

#define BUFFER_SIZE 10
void bbuff_init(void);
void bbuff_blocking_insert(void* item);
void* bbuff_blocking_extract(void);
_Bool bbuff_is_empty(void);


#endif //PRJ_BBUFF_H
