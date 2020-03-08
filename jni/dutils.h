#ifndef DUTILS_H_
#define DUTILS_H_

#include <stdint.h>
#include "dlogger.h"
#include "dcontainer.h"
#include <sys/mman.h>
#include <stdlib.h> 
#define MASK 0xFF

typedef char byte;
typedef void * p_void;

void uint_to_byte_array(uint32_t address, byte * b_array);
void place_bytes(void * source, void * dest, int len);

#endif