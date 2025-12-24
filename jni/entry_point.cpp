#include "dhook/dhook.h"
#include "entry_point.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

static const char * libName = "libil2cpp.so";
static ulong libBase = 0;

ulong getLibBase(const char* name) {
	FILE *fp;
	ulong addr = 0;
	char filename[32], buffer[1024];
	snprintf(filename, sizeof(filename), "/proc/%d/maps", getpid());
	fp = fopen(filename, "rt");
	if (fp != NULL) {
		while (fgets(buffer, sizeof(buffer), fp)) {
			if (strstr(buffer, name)) {
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

int (*_getDamageDealt)(int a1, int a2, int a3, int a4, int a5, int a6, int a7, void * a8, int a9, int a10, int a11);

int getDamageDealt(int a1, int a2, int a3, int a4, int a5, int a6, int a7, void * a8, int a9, int a10, int a11) {
    LOGD("DDBG", "Entering hooked func");
    return _getDamageDealt(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
}

__attribute__((constructor))
void libhook_main() {
    DHook((void *)solveASLRAddress(0x12CE338), (void *) getDamageDealt, (void **) &_getDamageDealt);
}
