#include <stdio.h>

#include "structures.h"
#include "parse_args.h"
#include "reactor_ltot_work.h"
#include "mega_log.h"

int main ( int argc, char * argv[] ) {

  run_mode_t run_mode;
  parse_args ( argc, argv, &run_mode );

  if ( -1 == run_reactor_ltot ( run_mode ) ) {

    ERROR_MSG ( "reactor failed\n" );
    return -1;
  }
 

  return 0;
}
