#include "pqueue.h"
#include "mega_log.h"
#include <memory.h>
#include <stdlib.h>

int init_pqueue ( pqueue_t * q, int exp_len, int size ) {


  q -> len = exp_len;


  q -> data = malloc ( size * q -> len );
  if ( NULL == q -> data ) {

    ERROR_MSG ( "memory poblem: malloc\n" );
    return -1;
  }


  memset ( q -> data, 0, sizeof (size * q -> len) );  
  q -> size = q -> head = q -> tail = 0;
  q -> capacity = size;

  return 0;
}

void push_pqueue ( pqueue_t * q, void * data ) {

  memcpy ( q -> data + q->tail * q -> len, data, q -> len );

  if ( ++q -> tail == q -> capacity )
    q -> tail = 0;
  q -> size += 1;
}

void pop_pqueue ( pqueue_t * q, void * data ) {

    memcpy ( data, q -> data + q -> head * q -> len, q -> len );
    if ( ++q -> head == q -> capacity )
      q -> head = 0;
    q -> size -= 1;    
}

void free_pqueue ( pqueue_t * q) {

  free ( q -> data );
  q -> data = NULL;  
}
