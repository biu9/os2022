.extern start_kernel

    .section .text.init
    .globl _start
_start:
    la t0, _traps
    la sp,_start # inin sp
    csrw stvec, t0
    addi sp,sp,-8*8
    sd a0,64(sp)
    sd a1,8(sp)
    sd a2,16(sp)
    sd a3,24(sp)
    sd a4,32(sp)
    sd a5,40(sp)
    sd a6,48(sp)
    sd a7,56(sp)

    # set sie[SIE]=1
    csrr t2,sie
    ori t2,t2,0x20
    csrw sie,t2

    # sbi_ecall(SBIT_SET_TIMER,0,next,0,0,0,0,0)
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

    #csrs sstatus, (1 << 1)
    csrr t2,sie
    ori t2,t2,2
    csrw sstatus,t2
    ld a0,64(sp)
    ld a1,8(sp)
    ld a2,16(sp)
    ld a3,24(sp)
    ld a4,32(sp)
    ld a5,40(sp)
    ld a6,48(sp)
    ld a7,56(sp)
    #addi sp,sp,64
    la sp,boot_stack_top
    j start_kernel

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 # <-- change to your stack size

    .globl boot_stack_top
boot_stack_top: