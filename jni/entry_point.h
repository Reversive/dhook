#ifndef ENTRY_POINT_H_
#define ENTRY_POINT_H_

#include <jni.h>
#include <unistd.h>

typedef unsigned long ulong;


const char * libName = "libil2cpp.so";
ulong libBase = 0;

ulong getLibBase(const char* libName);
ulong solveASLRAddress(ulong address);

ulong getLibBase(const char* libName) {
	FILE *fp;
	ulong addr = 0;
	char filename[32], buffer[1024];
	snprintf(filename, sizeof(filename), "/proc/%d/maps", getpid());
	fp = fopen(filename, "rt");
	if (fp != NULL) {
		while (fgets(buffer, sizeof(buffer), fp)) {
			if (strstr(buffer, libName)) {
				addr = (uintptr_t) strtoul(buffer, NULL, 16);
				break;
			}
		}
		fclose(fp);
	}
	return addr;
}

ulong solveASLRAddress(ulong address) {
	if (libBase == 0) {
		libBase = getLibBase(libName);
	}
	return (libBase + address);
}

#endif