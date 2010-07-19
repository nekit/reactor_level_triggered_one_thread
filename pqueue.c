#include "pqueue.h"
#include "mega_log.h"
#include <memory.h>
#include <stdlib.h>

int check_size ( pqueue_t * q, void * data ) {

    if ( sizeof (*data) != sizeof (*q -> exp) ) {

    ERROR_MSG ( "Different size of data in queue:\n in queue: %d\n data: %d\n", sizeof ( *q -> exp), sizeof (*data) );
    return -1;
  }
    return 0;
}

int init_pqueue ( pqueue_t * q, void * exp, int size ) {


  q -> exp = malloc ( sizeof (*exp) );

  INFO_MSG ( "q -> exp size = %d\n", sizeof (q -> exp) );
  INFO_MSG ( "int size = %d\n", sizeof (int) );
  
  
  if ( NULL == q -> exp ) {

    ERROR_MSG ( "memory poblem: malloc\n" );
    return -1;
  }

  q -> data = malloc ( size * sizeof ( *q -> exp ) );
  if ( NULL == q -> data ) {

    ERROR_MSG ( "memory poblem: malloc\n" );
    return -1;
  }


  memset ( q -> exp, 0, sizeof (*q -> exp) );
  memset ( q -> data, 0, sizeof (*q -> data) );  
  q -> size = q -> head = q -> tail = 0;
  q -> capacity = size;

  return 0;
}

void push_pqueue ( pqueue_t * q, void * data ) {

  if ( 0 != check_size ( q, data ) )
    return;

  memcpy ( q -> data + q->tail * sizeof(*q -> exp), data, sizeof (*q -> exp) );

  if ( ++q -> tail == q -> capacity )
    q -> tail = 0;
  q -> size += 1;
}

void pop_pqueue ( pqueue_t * q, void * data ) {

    if ( 0 != check_size ( q, data ) )
      return;

    memcpy ( data, q -> data + q -> head * sizeof (*q -> exp), sizeof (*q -> exp) );
    if ( ++q -> head == q -> capacity )
      q -> head = 0;
    q -> size -= 1;    
}

void free_pqueue ( pqueue_t * q) {

  free ( q -> exp );
  q -> exp = NULL;
  
  free ( q -> data );
  q -> data = NULL;  
}
