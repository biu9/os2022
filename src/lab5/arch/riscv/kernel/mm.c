#include "defs.h"
#include "string.h"
#include "mm.h"

#include "printk.h"

extern char _ekernel[];

struct {
    struct run *freelist;
} kmem;

uint64 kalloc() {
    struct run *r;

    r = kmem.freelist;
    kmem.freelist = r->next;
    
    memset((void *)r, 0x0, PGSIZE);
    return (uint64) r;
}

void kfree(uint64 addr) {
    struct run *r;

    // PGSIZE align 
    addr = addr & ~(PGSIZE - 1);

    memset((void *)addr, 0x0, (uint64)PGSIZE);

    r = (struct run *)addr;
    r->next = kmem.freelist;
    kmem.freelist = r;

    return ;
}

void kfreerange(char *start, char *end) {
    char *addr = (char *)PGROUNDUP((uint64)start);
    printk("start address: %lx, end address: %lx\n", (uint64)(addr) + PGSIZE, (uint64)(end));
    for (; (uint64)(addr) + PGSIZE <= (uint64)end; addr += PGSIZE) {
        kfree((uint64)addr);
    }
}

void mm_init(void) {
    printk("PHY_END + PA2VA_OFFSET: %lx + %lx = %lx\n", PHY_END, PA2VA_OFFSET, PHY_END + PA2VA_OFFSET);
    kfreerange(_ekernel, (char *)((PHY_END + PA2VA_OFFSET)));
    printk("...mm_init done!\n");
}
