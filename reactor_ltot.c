#include <stdio.h>

#include "structures.h"
#include "parse_args.h"

int main ( int argc, char * argv[] ) {

  run_mode_t run_mode;
  parse_args ( argc, argv, &run_mode );
 

  return 0;
}
