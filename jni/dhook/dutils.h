#ifndef DUTILS_H_
#define DUTILS_H_

#include <stdint.h>
#include <stddef.h>
#include "dlogger.h"
#include "dcontainer.h"
#include <sys/mman.h>
#include <stdlib.h> 
#define MASK 0xFF

typedef unsigned char byte;
typedef void * p_void;

#define _extern \
    extern "C" __attribute__((__visibility__("default")))


#endif