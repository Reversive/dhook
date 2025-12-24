#ifndef DCONTAINER_H_
#define DCONTAINER_H_

#define ARM32_INSTRUCTION_SIZE 4


enum MEMPROTECT_STATUS { MEMPROTECT_FAILED = -1, MEMPROTECT_SUCCESS };


typedef struct _DContainer {
    unsigned char branchDetourBytes[2 * ARM32_INSTRUCTION_SIZE];
    unsigned char restorationBytes[4 * ARM32_INSTRUCTION_SIZE];
} DContainer;



#endif