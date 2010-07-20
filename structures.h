#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdint.h>
#include "pqueue.h"

#define IP_ADDR_SIZE 20
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 1007
#define DEFAULT_MAX_USERS 2000
#define DEFULT_LISTN_BACKLOG 50
#define PACKET_SIZE sizeof ( uint32_t )
#define EPOLL_TIMEOUT (100)
#define NOT_SLEEP_TIMEOUT 0
#define DATA_QUEUE_SIZE 30

typedef char packet_t[ PACKET_SIZE ];

typedef struct run_mode_s {

  int port;
  char ip_addr[IP_ADDR_SIZE];
  int max_users;
  int listn_backlog;
  
} run_mode_t;

typedef enum {

  ST_SERVER,
  ST_DATA,
  ST_LAST,
  
} socket_type_t;


typedef struct sock_desk_s {

  int sock;
  int idx;
  socket_type_t type;
  packet_t send_pack;
  packet_t recv_pack;
  int send_ofs;
  int read_ofs;
  pqueue_t data_queue;
  int queue_init;
  
} sock_desk_t;

typedef struct reactor_s {

  int max_users;
  int listn_sock;
  int epfd;
  sock_desk_t * sd;
  pqueue_t slots_queue;
  
} reactor_t;

#endif /* End of STRUCTURES_H */
