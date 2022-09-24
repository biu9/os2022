#include "print.h"
#include "sbi.h"

void puts(char *s) {
    // unimplemented
    struct sbiret ret;
    res = sbi_ecall(0x1, 0x0, (uint64)s, 0, 0, 0, 0, 0);
    printf("%d",res.value);
}

void puti(int x) {
    // unimplemented
}
