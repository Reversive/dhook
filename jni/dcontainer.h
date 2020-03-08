#ifndef DCONTAINER_H_
#define DCONTAINER_H_

#define ARM32_INSTRUCTION_SIZE 4


typedef struct _DContainer {
    char branch_detour_bytes[2 * ARM32_INSTRUCTION_SIZE];
    char restoration_bytes[4 * ARM32_INSTRUCTION_SIZE];
} DContainer;



#endif