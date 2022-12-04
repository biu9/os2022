.extern start_kernel

    .section .text.init
    .globl _start
_start:
    la sp, boot_stack_top
    call setup_vm
    call relocate
    call mm_init
    call task_init
     la t0, _traps
     csrw stvec,t0
     addi t0,x0,32
     csrs sie,t0
     addi a0,x0,0
     addi a1,x0,0
     rdtime a2
     addi a3,x0,0
     addi a4,x0,0
     addi a5,x0,0
     addi a6,x0,0
     addi a7,x0,0
     ecall
     csrsi sstatus,2
    j start_kernel
    .section .bss.stack
    .globl boot_stack
relocate:
    # set ra = ra + PA2VA_OFFSET
    # set sp = sp + PA2VA_OFFSET (If you have set the sp before)
   
    #   YOUR CODE HERE   #

    # set satp with early_pgtbl
    
    #   YOUR CODE HERE   #
    
    # flush tlb
    sfence.vma zero, zero
		
    # flush icache
    fence.i
		
    ret

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 # <-- change to your stack size

    .globl boot_stack_top
boot_stack_top: