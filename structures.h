#ifndef STRUCTURES_H
#define STRUCTURES_H

#define IP_ADDR_SIZE 20
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 7

typedef struct run_mode_s {

  int port;
  char ip_addr[IP_ADDR_SIZE];
  
} run_mode_t;

#endif /* End of STRUCTURES_H */
