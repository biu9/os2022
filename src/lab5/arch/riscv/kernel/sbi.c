/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-09-20 17:12:36
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-10-26 20:48:14
 * @FilePath: /lab1/arch/riscv/kernel/sbi.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "types.h"
#include "sbi.h"


struct sbiret sbi_ecall(int ext, int fid, uint64 arg0,
			            uint64 arg1, uint64 arg2,
			            uint64 arg3, uint64 arg4,
			            uint64 arg5) 
{
	struct sbiret ret_val;
	__asm__ volatile (
		"mv a7, %[ext]\n"
		"mv a6, %[fid]\n"
		"mv a0, %[arg0]\n"
		"mv a1, %[arg1]\n"
		"mv a2, %[arg2]\n"
		"mv a3, %[arg3]\n"
		"mv a4, %[arg4]\n"
		"mv a5, %[arg5]\n"
		"ecall\n"
		"mv %[error] ,a0\n"
		"mv %[value] ,a1\n"
		: [error] "=r" (ret_val.error),[value] "=r" (ret_val.value)
		: [ext] "r" (ext), [fid] "r" (fid), [arg2] "r" (arg2), [arg3] "r" (arg3),
		  [arg4] "r" (arg4), [arg5] "r" (arg5), [arg0] "r" (arg0), [arg1] "r" (arg1)
		: "memory", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"
	);
	return ret_val;
}
