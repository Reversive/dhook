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

void ldr_pc_jump(byte * bytes, p_void target) {
    for(int i = 0; i < 4; i++) {
        bytes[i] = ldr_pc_pc_4[i]; // LDR PC, [PC, # - 0x4]
        bytes[i + 4] = ((uint32_t)target >> i * 8) & 0xFF; // [PC + 0x4] = detour_address
    }
}

p_void DHookTrampoline(p_void origin, p_void detour) {
    DContainer * dhc = (DContainer *)malloc(sizeof(DContainer)); 
    LOGD("DDBG", "::Ready to Hook: %p, Detour currently at: %p", origin, detour);
    ldr_pc_jump((byte*) dhc->branch_detour_bytes, detour);
    // Save first 8 bytes (2 instructions) from the original function to restore later in the trampoline
    place_bytes(origin, dhc->restoration_bytes, 8); 
    LOGD("DDBG", "::Saved Original Bytes");
    // Make sure we have perms to patch the origin
    p_void page_start = (p_void)((uint32_t)origin - (uint32_t)origin % PAGE_SIZE); // eq. to page_start = addr & ~(PAGE_SIZE-1);
    if(mprotect((p_void)page_start, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        LOGE("DERR", "::mprotect failed step 1");
        return NULL;
    }
    // Place the branch to our detour function on origin
    place_bytes(dhc->branch_detour_bytes, origin, 8);
    LOGD("DDBG", "::Placed Detour");
    uint32_t return_address = ((uint32_t)origin + 0x8); // This is were we want to return after we finish with our detour code 

    // dhc->restoration_bytes contain the original function bytes in the first 8 bytes, the remaining 8 bytes are used to branch back to origin+0x8
    ldr_pc_jump((byte*)((uint32_t)dhc->restoration_bytes + 8), (p_void)return_address); 

    page_start = (p_void)((uint32_t)dhc->restoration_bytes - (uint32_t)dhc->restoration_bytes % PAGE_SIZE); // Make sure we have perms to execute the trampoline
    if(mprotect((p_void)page_start, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        LOGE("DERR", "::mprotect failed step 2");
        return NULL;
    }
    LOGD("DDBG", "::Trampoline Set");
    return (p_void) dhc->restoration_bytes;

}


void DHook(p_void origin, p_void detour, p_void * trampoline) { 
    if ((reinterpret_cast<uintptr_t>(origin) & 0x1) == 0) {
        *trampoline = DHookTrampoline(origin, detour);
    }  else {
        LOGE("DERR", "::Lib supports ARM32-only for now");
    }
    
}