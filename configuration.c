

#include "configuration.h"

#ifdef __GNUC__
  __attribute__ ((section(".configuration")))
#else
  #pragma location = "configuration"
#endif
CONF conf;

