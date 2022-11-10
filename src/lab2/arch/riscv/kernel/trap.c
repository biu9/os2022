#include"printk.h"

void trap_handler(unsigned long scause, unsigned long sepc) {
        if(scause >= 0x8000000000000000) {
             if(scause == 0x8000000000000005) {
                printk("[S] Supervisor Mode Timer Interrupt\n");
                clock_set_next_event();
             } else {
                printk("not timer interrupt\n");
             }
        } else {
             printk("other\n");
        }
        return;
}