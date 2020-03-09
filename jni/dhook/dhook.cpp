#include "dhook.h"



/* 
    On runtime PC points two instructions ahead (PC + 0x8), what we do here is point PC to the next instruction (PC + 0x8 - 0x4 = PC + 0x4),
    Read the 4 bytes (word) at that location and load them into PC, that will trigger a branch into that address.
    LDR PC, [PC, # - 0x4]
    HEX : E5 1F F0 04
    BIN : 1110 01 010001 1111 1111 000000000100‬
    Check https://iitd-plos.github.io/col718/ref/arm-instructionset.pdf page 4-26

*/
byte ldr_pc_pc_4[4] = { 0x04, 0xF0, 0x1F, 0xE5 }; 
void copyBranchBytes(byte * bytes, p_void target) {
    for(int i = 0; i < 4; i++) {
        bytes[i] = ldr_pc_pc_4[i]; 
        bytes[i + 4] = ((uint32_t)target >> i * 8) & 0xFF; 
    }
}

// Set @protection flags on @target starting page, @return mprotect response parsed as MEMPROTECT_STATUS
MEMPROTECT_STATUS setPageProtection(void * target, int protection) {
    p_void page_start = (p_void)((uint32_t)target - (uint32_t)target % PAGE_SIZE);
    return (MEMPROTECT_STATUS) mprotect((p_void)page_start, PAGE_SIZE, protection);
}

// Set @len bytes on @dest from @source
void placeBytes(void * source, void *  dest, int len) {
    for(int i = 0; i < len; i++) *(byte *)((uint32_t)dest + i) = *(byte *)((uint32_t)source + i);
}

p_void DHookTrampolineARM(p_void origin, p_void target) {
    DContainer * dhc = (DContainer *)malloc(sizeof(DContainer)); 
    LOGD("DDBG", "::Ready to Hook: %p, Detour currently at: %p", origin, target);

    copyBranchBytes(dhc->branchDetourBytes, target);
    placeBytes(origin, dhc->restorationBytes, 8); // Save first 8 bytes (2 instructions) from the original function to restore later in the trampoline
    LOGD("DDBG", "::Saved Original Bytes");

    MEMPROTECT_STATUS status = setPageProtection(origin, PROT_READ | PROT_WRITE | PROT_EXEC);
    if(status == MEMPROTECT_FAILED) {
        LOGE("DERR", "::mprotect failed on step 1");
        return NULL;
    }

    placeBytes(dhc->branchDetourBytes, origin, 8); // Place the jump to our detour function on origin
    LOGD("DDBG", "::Placed Detour");

    p_void returnAddress = (p_void)((uint32_t)origin + 0x8); // This is were we want to return after we finish with our detour code 
    copyBranchBytes(dhc->restorationBytes + 8, returnAddress); // dhc->restorationBytes contain the original function bytes in the first 8 bytes, the remaining 8 bytes are used to jump back to origin+0x8
    status = setPageProtection((void *)dhc->restorationBytes, PROT_READ | PROT_WRITE | PROT_EXEC);
    if(status == MEMPROTECT_FAILED) {
        LOGE("DERR", "::mprotect failed on step 2");
        return NULL;
    }

    LOGD("DDBG", "::Trampoline Set");
    return (p_void) dhc->restorationBytes;
}

void DHook(p_void origin, p_void detour, p_void * trampoline) { 
    if ((reinterpret_cast<uintptr_t>(origin) & 0x1) == 0) {
        *trampoline = DHookTrampolineARM(origin, detour);
    }  else {
        LOGE("DERR", "::Lib supports ARM32-only for now");
    }
    
}