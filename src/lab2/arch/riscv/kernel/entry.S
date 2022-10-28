    .section .text.entry
    .align 2
    .globl _traps 
_traps:
    # YOUR CODE HERE
    # -----------

        # 1. save 32 registers and sepc to stack
        csrrw x2,sscratch,x2
        addi x2,x2,-32*8
        sd x1,0(x2)
        sd x3,8(x2)
        sd x4,16(x2)
        sd x5,24(x2)
        sd x6,32(x2)
        sd x7,40(x2)
        sd x8,48(x2)
        sd x9,56(x2)
        sd x10,64(x2)
        sd x11,72(x2)
        sd x12,80(x2)
        sd x13,88(x2)
        sd x14,96(x2)
        sd x15,104(x2)
        sd x16,112(x2)
        sd x17,120(x2)
        sd x18,128(x2)
        sd x19,136(x2)
        sd x20,144(x2)
        sd x21,152(x2)
        sd x22,160(x2)
        sd x23,168(x2)
        sd x24,176(x2)
        sd x25,184(x2)
        sd x26,192(x2)
        sd x27,200(x2)
        sd x28,208(x2)
        sd x29,216(x2)
        sd x30,224(x2)
        sd x31,232(x2)
        csrr t0,sepc
        sd t0,240(x2)


    # -----------

        # 2. call trap_handler

    # -----------

        # 3. restore sepc and 32 registers (x2(sp) should be restore last) from stack
        ld t0,240(x2)
        csrw sepc,t0
        ld x1,0(x2)
        ld x3,8(x2)
        ld x4,16(x2)
        ld x5,24(x2)
        ld x6,32(x2)
        ld x7,40(x2)
        ld x8,48(x2)
        ld x9,56(x2)
        ld x10,64(x2)
        ld x11,72(x2)
        ld x12,80(x2)
        ld x13,88(x2)
        ld x14,96(x2)
        ld x15,104(x2)
        ld x16,112(x2)
        ld x17,120(x2)
        ld x18,128(x2)
        ld x19,136(x2)
        ld x20,144(x2)
        ld x21,152(x2)
        ld x22,160(x2)
        ld x23,168(x2)
        ld x24,176(x2)
        ld x25,184(x2)
        ld x26,192(x2)
        ld x27,200(x2)
        ld x28,208(x2)
        ld x29,216(x2)
        ld x30,224(x2)
        ld x31,232(x2)
        addi x2,x2,32*8
        csrrw x2,sscratch,x2

    # -----------

        # 4. return from trap

    # -----------