#include <memory.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "structures.h"
#include "parse_args.h"
#include "mega_log.h"

int parse_args ( int argc, char * argv[], run_mode_t * rm ) {
  
  strcpy(rm -> ip_addr , DEFAULT_IP);
  rm -> port = DEFAULT_PORT;

  int res;  
  while ( (res = getopt(argc,argv,"s:p:L:")) != -1) {
    switch (res){
       case 's':
	 stpcpy(rm -> ip_addr, optarg);
	 DEBUG_MSG ( "ip: %s\n", rm -> ip_addr );
	 break;
	 
       case 'p':
	 if (0 < atoi(optarg))
	   rm -> port = atoi(optarg);
	 else{
	   printf("wrong port \n");
	   return -1;
	 }      
	 break;
    
    case 'h':
      printf ( "reqiers arguments:\n -s: ip-address\n -p: port\n");
      return -1;
    
    case 'L' :
      TRACE_MSG( "parsing level %s\n", optarg );
      INIT_LOG_LEVEL( optarg );
      break;
    }
    
  }
  
  return 0;
}
