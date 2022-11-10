#include "sbi.h"

unsigned long TIMECLOCK = 100000000;

unsigned long get_cycles() {
        uint64 res;
        __asm__ __volatile__("rdtime %0" : "=r"(res));
        return res;
}

void clock_set_next_event() {
        unsigned long next = get_cycles() + TIMECLOCK;
        sbi_ecall(0,0,next,0,0,0,0,0);
}