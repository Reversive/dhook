#include "dhook/dhook.h"
#include "entry_point.h"

int (*_getDamageDealt)(int a1, int a2, int a3, int a4, int a5, int a6, int a7, void * a8, int a9, int a10, int a11);

int getDamageDealt(int a1, int a2, int a3, int a4, int a5, int a6, int a7, void * a8, int a9, int a10, int a11) {

    LOGD("DDBG", "Entering hooked func");
    return _getDamageDealt(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
 }


__attribute__((constructor))
void libhook_main() {

    DHook((void *)solveASLRAddress(0x12CE338), (void *) getDamageDealt, (void **) &_getDamageDealt );
   
}