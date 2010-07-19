#ifndef HANDLE_EVENT_H
#define HANDLE_EVENT_H

#include <sys/epoll.h>
#include "structures.h"

int handle_event ( struct epoll_event * ev, reactor_t * rct );

#endif /* End of HANDLE_EVENT_H */
