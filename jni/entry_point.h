#ifndef ENTRY_POINT_H_
#define ENTRY_POINT_H_

#include <jni.h>
#include <unistd.h>

typedef unsigned long ulong;

ulong getLibBase(const char* libName);
ulong solveASLRAddress(ulong address);

#endif
