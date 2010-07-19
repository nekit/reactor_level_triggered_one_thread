#include "syn_pqueue.h"
#include "mega_log.h"

int init_syn_pqueue ( syn_pqueue_t * q, void * exp, int size ) {

  if ( 0 != pthread_mutex_init ( &q -> read_mutex, NULL ) ) {

    ERROR_MSG ( "mutex init failed\n" );
    return -1;
  }

  if ( 0 != pthread_mutex_init ( &q -> write_mutex, NULL ) ) {

    ERROR_MSG ( "mutex init failed\n" );
    return -1;
  }

  return init_pqueue ( &q -> queue, exp, size );  
}

void push_syn_pqueue ( syn_pqueue_t * q, void * data ) {

  pthread_mutex_lock ( &q -> write_mutex );
  push_pqueue ( &q -> queue, data );
  pthread_mutex_unlock ( &q -> write_mutex );
}

void pop_syn_pqueue ( syn_pqueue_t * q, void * data ) {

  pthread_mutex_lock ( &q -> read_mutex );
  pop_pqueue ( &q -> queue, data );
  pthread_mutex_unlock ( &q -> read_mutex );
}

void free_syn_pqueue ( syn_pqueue_t * q) {

  free_pqueue ( &q -> queue );
}
