#include <memory.h>
#include <stdlib.h>

#include "reactor_ltot_work.h"
#include "socket_operations.h"
#include "arpa/inet.h"
#include "sys/epoll.h"
#include "mega_log.h"
#include "handle_event.h"

#define SERVER_SOCKET_DESK_ID 0

int init_reactor ( reactor_t * r, const run_mode_t * rm ) {

  int listn_sock = bind_socket ( inet_addr ( rm -> ip_addr ), htons ( rm -> port ), rm -> listn_backlog );
  if ( -1 == listn_sock )
    return -1;

  r -> epfd = epoll_create ( rm -> max_users + 1 );
  if ( -1 == r -> epfd ) {

    ERROR_MSG ( "epoll create failed\n" );
    return -1;
  }

  r -> sd = malloc ( (rm -> max_users + 1) * sizeof (sock_desk_t) );
  if ( NULL == r -> sd ) {

    ERROR_MSG ( "memory problems: malloc\n" );
    return -1;
  }

  memset ( r -> sd, 0, sizeof (r -> sd) );
  r -> sd[SERVER_SOCKET_DESK_ID].sock = listn_sock;
  r -> sd[SERVER_SOCKET_DESK_ID].type = ST_SERVER;
  r -> listn_sock = listn_sock;

  struct epoll_event ev;
  memset ( &ev, 0, sizeof (ev) );
  ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
  ev.data.ptr = &r -> sd[SERVER_SOCKET_DESK_ID];
  if ( -1 == epoll_ctl ( r -> epfd, EPOLL_CTL_ADD, listn_sock, &ev ) ) {

    ERROR_MSG ( "epoll_ctl failed\n" );
    return -1;
  }

  int i = 0;
  if ( -1 == init_pqueue ( &r -> slots_queue, sizeof(i), rm -> max_users ) ) {

    ERROR_MSG ( "initing slots queue problem\n" );
    return -1;
  }

  for ( i = 1; i <= rm -> max_users; ++i )
    push_pqueue ( &r -> slots_queue, &i );

  r -> max_users = rm -> max_users;

  return 0;
}

void free_reactor ( reactor_t * r ) {

  int i;
  for ( i = 0; i <= r -> max_users; ++i )
    if ( i != SERVER_SOCKET_DESK_ID  )
      if ( r -> sd[i].data_queue.data != NULL)
	free_pqueue ( &r -> sd[i].data_queue );

  free ( r -> sd );
  r -> sd = NULL;

  free_pqueue ( &r -> slots_queue );
}

int run_reactor_ltot ( run_mode_t run_mode ) {


  reactor_t reactor;
  struct epoll_event events[run_mode.max_users + 1];
  
  if ( -1 == init_reactor( &reactor, &run_mode ) ) {

    ERROR_MSG ( "init reactor failed\n" );
    return -1;
  }

  for ( ; ; ) {

    TRACE_MSG ( "epoll waiting...\n" );
    int n = epoll_wait ( reactor.epfd, events, sizeof (events) / sizeof (events[0]), NOT_SLEEP_TIMEOUT );

    if ( -1 == n ) {

      ERROR_MSG ( "epoll wait failed: n == %d\n", n );
      return -1;
    }


    int i;
    for ( i = 0; i < n; ++i )
      handle_event ( &events[i], &reactor );    
    
  }

  free_reactor( &reactor );

  return 0;
}
