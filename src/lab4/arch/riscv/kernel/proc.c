/*
 * @Author: Theta 1467116498@qq.com
 * @Date: 2022-11-10 21:18:01
 * @LastEditors: Theta 1467116498@qq.com
 * @LastEditTime: 2022-11-12 12:29:56
 * @FilePath: /os22fal-stu/src/lab3/arch/riscv/kernel/proc.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//arch/riscv/kernel/proc.c
#include "proc.h"
#include "mm.h"
#include "defs.h"
#include "rand.h"
extern void __dummy();
extern void __switch_to(struct task_struct* prev, struct task_struct* next);

struct task_struct* idle;           // idle process
struct task_struct* current;        // 指向当前运行线程的 `task_struct`
struct task_struct* task[NR_TASKS]; // 线程数组, 所有的线程都保存在此

void task_init() {
    // 1. 调用 kalloc() 为 idle 分配一个物理页
    // 2. 设置 state 为 TASK_RUNNING;
    // 3. 由于 idle 不参与调度 可以将其 counter / priority 设置为 0
    // 4. 设置 idle 的 pid 为 0
    // 5. 将 current 和 task[0] 指向 idle

    /* YOUR CODE HERE */

    // 1. 调用 kalloc() 为 idle 分配一个物理页
    idle = kalloc();
    // 2. 设置 state 为 TASK_RUNNING;
    idle->state = TASK_RUNNING;
    // 3. 由于 idle 不参与调度 可以将其 counter / priority 设置为 0
    idle->counter = 0;
    idle->priority = 0;
    // 4. 设置 idle 的 pid 为 0
    idle->pid = 0;
    // 5. 将 current 和 task[0] 指向 idle
    current = idle;
    task[0] = idle;

    // 1. 参考 idle 的设置, 为 task[1] ~ task[NR_TASKS - 1] 进行初始化
    // 2. 其中每个线程的 state 为 TASK_RUNNING, counter 为 0, priority 使用 rand() 来设置, pid 为该线程在线程数组中的下标。
    // 3. 为 task[1] ~ task[NR_TASKS - 1] 设置 `thread_struct` 中的 `ra` 和 `sp`,
    // 4. 其中 `ra` 设置为 __dummy （见 4.3.2）的地址,  `sp` 设置为 该线程申请的物理页的高地址

    /* YOUR CODE HERE */
    // 1. 参考 idle 的设置, 为 task[1] ~ task[NR_TASKS - 1] 进行初始化
    for(int i=1;i<=NR_TASKS - 1;i++){
        task[i] = kalloc();
        // 2. 其中每个线程的 state 为 TASK_RUNNING, counter 为 0, priority 使用 rand() 来设置, pid 为该线程在线程数组中的下标。
        task[i]->state = TASK_RUNNING;
        task[i]->counter = 0;
        task[i]->priority = rand();
        task[i]->pid = i;
        
        // 3. 为 task[1] ~ task[NR_TASKS - 1] 设置 `thread_struct` 中的 `ra` 和 `sp`,
        uint64 ra_addr;
        __asm__ volatile (
            "la t0, __dummy\n"
            "mv %[ra_addr],t0\n"
            
            : [ra_addr] "=r" (ra_addr)
            : 
            : "memory", "t0"
        );
        task[i]->thread.ra = ra_addr;
        task[i]->thread.sp = (uint64)task[i] + PGSIZE;
        
    }
    printk("...proc_init done!\n");
}

void dummy() {
    uint64 MOD = 1000000007;
    uint64 auto_inc_local_var = 0;
    int last_counter = -1;
    while(1) {
        if (last_counter == -1 || current->counter != last_counter) {
            last_counter = current->counter;
            auto_inc_local_var = (auto_inc_local_var + 1) % MOD;
            printk("[PID = %d] is running. auto_inc_local_var = %d thread space begin at %lx \n", current->pid, auto_inc_local_var,current);
        }
    }
}

void switch_to(struct task_struct* next) {
    /* YOUR CODE HERE */
    struct task_struct* prev = current;
    if(next->pid != current->pid){
        current = next;
        __switch_to(prev,next);
    }
}

void do_timer(void) {
    // 1. 如果当前线程是 idle 线程 直接进行调度
    // 2. 如果当前线程不是 idle 对当前线程的运行剩余时间减1 若剩余时间仍然大于0 则直接返回 否则进行调度

    /* YOUR CODE HERE */
    if(current == idle) schedule();
    else if(--current->counter <= 0) schedule();
}

void schedule(void) {
    /* YOUR CODE HERE */
#ifdef SJF
    struct task_struct* next = current;
    for(int i=1;i<=NR_TASKS - 1;i++){
        if(task[i]->state == TASK_RUNNING && task[i]->counter > 0){
            if(next == current) next = task[i];
            else if(task[i]->counter < next->counter) next = task[i];
        }
    }

    if(next->counter == 0){
        for(int i=1;i<=NR_TASKS - 1;i++){
            task[i]->counter = rand();
            printk("SET [PID = %d COUNTER = %d]\n",task[i]->pid,task[i]->counter);
        }
        schedule();
    }
    else{
        printk("\nswitch to [PID = %d COUNTER = %d]\n",next->pid,next->counter);
        switch_to(next);
    }
#else
    struct task_struct* next = current;
    for(int i=1;i<=NR_TASKS - 1;i++){
        if(task[i]->state == TASK_RUNNING && task[i]->counter > 0){
            if(next == current) next = task[i];
            else if(task[i]->priority > next->priority) next = task[i];
        }
    }

    if(next->counter == 0){
        for(int i=1;i<=NR_TASKS - 1;i++){
            task[i]->counter = rand();
            printk("SET [PID = %d COUNTER = %d]\n",task[i]->pid,task[i]->counter);
        }
        schedule();
    }
    else{
        printk("\nswitch to [PID = %d COUNTER = %d]\n",next->pid,next->counter);
        switch_to(next);
    }
#endif
}