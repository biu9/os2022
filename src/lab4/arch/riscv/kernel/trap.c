/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-10-26 20:46:21
 * @LastEditors: Theta 1467116498@qq.com
 * @LastEditTime: 2022-11-12 12:18:43
 * @FilePath: /lab2/arch/riscv/kernel/trap.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "types.h"
#include "sbi.h"
#include "printk.h"
#include "proc.h"

void trap_handler(unsigned long scause, unsigned long sepc) {
    // 通过 `scause` 判断trap类型
    // 1. 如果是interrupt 判断是否是timer interrupt
    // 1.1 如果是timer interrupt 则打印: "[S] Supervisor Mode Timer Interrupt"
    //     并通过 `clock_set_next_event()` 设置下一次时钟中断
    //     关于 `clock_set_next_event()` 见 4.5 节
    // 1.2 & 2. 其他interrupt / exception 可以直接忽略
    
    // YOUR CODE HERE
    if(((scause >> 63) == 1) && ((scause << 1) == 10)){
        clock_set_next_event();
        do_timer();
    }
}