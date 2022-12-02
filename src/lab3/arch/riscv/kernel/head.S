.extern start_kernel

    .section .text.init
    .globl _start
_start:
    la sp, boot_stack_top
    call mm_init
    call task_init
    # ----- lab3 code begin -----
     # set stvec = _traps
     la t0, _traps
     csrw stvec,t0
     # set sie[STIE] = 1
     addi t0,x0,32
     csrs sie,t0
     # set first time interrupt
     addi a0,x0,0
     addi a1,x0,0
     rdtime a2
     addi a3,x0,0
     addi a4,x0,0
     addi a5,x0,0
     addi a6,x0,0
     addi a7,x0,0
     ecall

    # set sstatus[SIE] = 1
     csrsi sstatus,2
    # ----- lab3 code end -----

    
    j start_kernel
    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 # <-- change to your stack size

    .globl boot_stack_top
boot_stack_top: