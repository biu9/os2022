.extern start_kernel
.extern early_pgtbl

    .section .text.init
    .globl _start
_start:  
    la sp, boot_stack_top
    call setup_vm
    call relocate
    call mm_init
    call setup_vm_final
    call task_init
    
    la t0, _traps
    csrw stvec, t0
    li t0,0x32
    csrs sie,t0

    rdtime a0
    addi a0,a0,10
    li a2, 0
    li a1, 0
    li a3, 0
    li a4, 0
    li a5, 0
    li a6, 0
    li a7, 0
    ecall
    csrsi sstatus,2

    j start_kernel
relocate:
    # set ra = ra + PA2VA_OFFSET
    # set sp = sp + PA2VA_OFFSET (If you have set the sp before)
   
    #   YOUR CODE HERE   #
    li t0, 0xffffffdf80000000
    add ra, ra, t0
    add sp, sp, t0

    # set satp with early_pgtbl
    
    #   YOUR CODE HERE   #
    la t0,early_pgtbl
    srli t0,t0,12
    li t1,0x8000000000000000
    or t0,t0,t1
    csrw satp,t0
    
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