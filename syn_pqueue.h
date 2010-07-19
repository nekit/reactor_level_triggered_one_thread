#ifndef SYN_PQUEUE_H
#define SYN_PQUEUE_H

#include "pqueue.h"
#include <pthread.h>

typedef struct syn_pqueue_s {

  pqueue_t queue;
  pthread_mutex_t read_mutex;
  pthread_mutex_t write_mutex;
  
} syn_pqueue_t;

/*
  return -1 if error occurs
*/
int init_syn_pqueue ( syn_pqueue_t * q, void * exp, int size );

void push_syn_pqueue ( syn_pqueue_t * q, void * data );
void pop_syn_pqueue ( syn_pqueue_t * q, void * data );

void free_syn_pqueue ( syn_pqueue_t * q);

#endif /* End of SYN_PQUEUE_H */
