.section .text.entry
    .align 2
    .globl _traps 
_traps:
    # YOUR CODE HERE
    # -----------

        # 1. save 32 registers and sepc to stack
        addi sp,sp,-136
        sd ra,128(sp)
        sd t0,112(sp)

        csrr t0,sepc
        sd t0,120(sp)
        
        sd t1,104(sp)
        sd t2,96(sp)
        sd a0,88(sp)
        sd a1,80(sp)
        sd a2,72(sp)
        sd a3,64(sp)
        sd a4,56(sp)
        sd a5,48(sp)
        sd a6,40(sp)
        sd a7,32(sp)
        sd t3,24(sp)
        sd t4,16(sp)
        sd t5,8(sp)
        sd t6,0(sp)

    # -----------

        # 2. call trap_handler
        csrr a0,scause
        csrr a1,sepc
        call trap_handler

    # -----------

        # 3. restore sepc and 32 registers (x2(sp) should be restore last) from stack
        ld ra,128(sp)

        ld t0,120(sp)
        csrw sepc,t0
        
        ld t0,112(sp)
        ld t1,104(sp)
        ld t2,96(sp)
        ld a0,88(sp)
        ld a1,80(sp)
        ld a2,72(sp)
        ld a3,64(sp)
        ld a4,56(sp)
        ld a5,48(sp)
        ld a6,40(sp)
        ld a7,32(sp)
        ld t3,24(sp)
        ld t4,16(sp)
        ld t5,8(sp)
        ld t6,0(sp)

        addi sp,sp,136
    # -----------

        # 4. return from trap
        sret

    # -----------

    .globl __dummy
__dummy:
    # YOUR CODE HERE
    # -----------
        la t0,dummy
        csrw sepc,t0
        sret
    .globl __switch_to

__switch_to:
        li    a4,  40 
        add   a3, a0, a4
        add   a4, a1, a4
    # save state to prev process
    # YOUR CODE HERE
        sd  ra,8*0(a3)
        sd  sp,8*1(a3)
        sd  s0,8*2(a3)
        sd  s1,8*3(a3)
        sd  s2,8*4(a3)
        sd  s3,8*5(a3)
        sd  s4,8*6(a3)
        sd  s5,8*7(a3)
        sd  s6,8*8(a3)
        sd  s7,8*9(a3)
        sd  s8,8*10(a3)
        sd  s9,8*11(a3)
        sd  s10,8*12(a3)
        sd  s11,8*13(a3)

    # restore state from next process
    # YOUR CODE HERE
        ld  ra,8*0(a4)
        ld  sp,8*1(a4)
        ld  s0,8*2(a4)
        ld  s1,8*3(a4)
        ld  s2,8*4(a4)
        ld  s3,8*5(a4)
        ld  s4,8*6(a4)
        ld  s5,8*7(a4)
        ld  s6,8*8(a4)
        ld  s7,8*9(a4)
        ld  s8,8*10(a4)
        ld  s9,8*11(a4)
        ld  s10,8*12(a4)
        ld  s11,8*13(a4)
        ret