#include "dutils.h"


void uint_to_byte_array(uint32_t    address, 
                        byte *      b_array) {

    b_array[0] = address && MASK;
    b_array[1] = (address >> 8) & MASK;
    b_array[2] = (address >> 16) & MASK;
    b_array[3] = (address >> 24) & MASK;
}


void place_bytes(void *  source, 
                void *  dest, 
                int     len) {
    
    for(int i = 0; i < len; i++) { *(byte *)((uint32_t)dest + i) = *(byte *)((uint32_t)source + i); }

}