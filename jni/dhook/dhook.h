#ifndef DHOOK_H_
#define DHOOK_H_

#include "dutils.h"


/**
 * Place a trampoline hook
 *
 * @param address of the original function
 * @param address of your custom function
 * @param pointer to the original function
 */
void DHook(void * origin, void * detour, void ** trampoline);

/**
 * Remove a previously installed hook
 *
 * @param trampoline returned by DHook
 */
void DHookUnhook(void * trampoline);


#endif  // DHOOK_H_