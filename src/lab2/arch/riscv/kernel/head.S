extern start_kernel

    .section .text.init         <- 将 _start 放入.text.init section 
    .globl _start
_start:
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