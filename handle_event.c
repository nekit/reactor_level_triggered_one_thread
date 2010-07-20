#include "handle_event.h"
#include "mega_log.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <memory.h>
#include <fcntl.h>

int set_nonblock ( int sock ) {

  if ( -1 == fcntl ( sock, F_SETFL, O_NONBLOCK ) )
   return -1;

  return 0;
}

void init_sock_desk ( sock_desk_t * ds, int sock, int idx ) {

  TRACE_MSG ( "initing socket deskriptor for socket %d\n", sock );

  ds -> sock = sock;
  ds -> send_ofs = sizeof ( packet_t );
  ds -> read_ofs = 0;
  ds -> idx = idx;
  if ( 0 == ds -> queue_init ) {

    TRACE_MSG ( "initing pqueue\n" );
    packet_t tmp;
    init_pqueue ( &ds -> data_queue, sizeof (tmp), DATA_QUEUE_SIZE );
  }else {

    pqueue_t * tpq = &ds -> data_queue;
    tpq -> size = tpq -> head = tpq -> tail = 0;
  }

 TRACE_MSG ( "socket deskriptor inited\n");

}

int handle_error ( struct epoll_event * ev, reactor_t * rct ) {

  DEBUG_MSG ( "handling error\n" );

  sock_desk_t * ds = ev -> data.ptr;
  epoll_ctl ( rct -> epfd, EPOLL_CTL_DEL, ds -> sock, NULL );
  ds -> sock = -1;
  push_pqueue ( &rct -> slots_queue, &ds -> idx );

  return 0;
}


int handle_accept_event ( reactor_t * rct ) {

  static int cnt = 1;  

  DEBUG_MSG ( "accepting\n" );

  if ( rct -> slots_queue.size == 0 ) {

    //WARN_MSG ( "can't accept client\n maximum clients provided\n" );
    return -1;
  }

  int idx = -1;
  pop_pqueue ( &rct -> slots_queue, &idx );
  
  int sock = accept ( rct -> listn_sock, NULL, NULL );

  DEBUG_MSG ( "accepted socket %d\n", sock );

  if ( -1 == set_nonblock ( sock ) ) {

    ERROR_MSG ( "set nonblocking socket %d failed\n", sock );
    return -1;
  }

  init_sock_desk ( &rct -> sd[idx], sock, idx );
  
  struct epoll_event ev;
  memset ( &ev, 0, sizeof (ev) );
  ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;
  ev.data.ptr = &rct -> sd[idx];
  epoll_ctl ( rct -> epfd, EPOLL_CTL_ADD, sock, &ev);

  INFO_MSG ( "accepted successfully %d\n", cnt++ );

  return 0;
}

int handle_read_event ( struct epoll_event * ev, reactor_t * rct ) {

  if ( ((sock_desk_t *)ev -> data.ptr) -> sock == rct -> listn_sock ) {
    handle_accept_event ( rct );
    return 0;
  }

  DEBUG_MSG ( "handling read event\n" );

  sock_desk_t * dp = ev -> data.ptr;

  if ( dp -> data_queue.size == dp -> data_queue.capacity ) {

    DEBUG_MSG ( "data queue is full on sock %d\n", dp -> sock );
    return 0;
  }
  
  int len = recv ( dp -> sock, dp -> recv_pack + dp -> read_ofs, sizeof (dp -> recv_pack) - dp -> read_ofs, 0 );
  if ( len <= 0 ) {

    ERROR_MSG ( "recieve error on sock %d\n", dp -> sock );
    handle_error ( ev, rct );
    return -1;
  }

  dp -> read_ofs += len;
  if ( sizeof (dp -> recv_pack) == dp -> read_ofs ) {

    push_pqueue ( &dp -> data_queue, &dp -> recv_pack );
    dp -> read_ofs = 0;

    DEBUG_MSG ( "packet recieved on sock %d\n", dp -> sock );
  }  

  return 0;
}

int handle_write_event ( struct epoll_event * ev, reactor_t * rct ) {

  DEBUG_MSG ( "handling write event\n" );

  sock_desk_t * dp = ev -> data.ptr;

  if ( 0 == dp -> data_queue.size && sizeof (dp -> send_pack) == dp -> send_ofs ) {

    // WARN_MSG ( "nothing to send on sock\n", dp -> sock );
    return 0;
  }

  if ( sizeof (dp -> send_pack) == dp -> send_ofs ) {

    pop_pqueue ( &dp -> data_queue, &dp -> send_pack );
    dp -> send_ofs = 0;    
  }

  int len = send ( dp -> sock, dp -> send_pack + dp -> send_ofs, sizeof ( dp -> send_pack ) - dp -> send_ofs, 0 );
  dp -> send_ofs += len;

  return 0;
}


int handle_event ( struct epoll_event * ev, reactor_t * rct ) {

  if ( 0 != (ev -> events & EPOLLIN) )
    handle_read_event ( ev, rct );

  if ( 0 != (ev -> events & EPOLLOUT) )
    handle_write_event ( ev, rct );

  if ( 0 != (ev -> events & (EPOLLERR | EPOLLHUP) ) )
    handle_error ( ev, rct );


  return 0;
}
