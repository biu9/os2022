.extern start_kernel

    .section .text.init
    .globl _start
_start:

    # YOUR CODE HERE

    # ------------------
        
        # set stvec = _traps
        la t0, _traps
        csrw stvec, t0
    
    # ------------------
    
        # set sie[STIE] = 1
        csrs sie,(1 << 1)
    
    # ------------------
    
        # set first time interrupt
    
    # ------------------
    
        # set sstatus[SIE] = 1
        csrs sstatus, (1 << 1)

    # ------------------
    # - your code here -
    # ------------------

    la sp,boot_stack_top
    j start_kernel

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 # <-- change to your stack size

    .globl boot_stack_top
boot_stack_top: