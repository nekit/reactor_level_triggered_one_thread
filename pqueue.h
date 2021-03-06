#ifndef PQUEUE_H
#define PQUEUE_H

typedef struct pqueue_s {

  int len;
  void * data;
  int head;
  int tail;
  int capacity;
  int size;
  
} pqueue_t;

/*
  return -1 if error occurs
*/
int init_pqueue ( pqueue_t * q, int exp_len, int size );

void push_pqueue ( pqueue_t * q, void * data );
void pop_pqueue ( pqueue_t * q, void * data );

void free_pqueue ( pqueue_t * q);

#endif /* End of PQUEUE_H */
