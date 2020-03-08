#include "dhook.h"


int (*_myHookTestFunc)(int a1);
int myHookTestFunc(int a1) {
    LOGD("DDBG", ":: Inside my hook test function");
    return _myHookTestFunc(a1);
}


__attribute__((constructor))
void libhook_main() {

    DHook((void *)0x00004000, (void *) myHookTestFunc, (void **) &_myHookTestFunc );
   
}