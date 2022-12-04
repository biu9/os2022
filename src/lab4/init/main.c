/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-09-20 17:12:36
 * @LastEditors: Theta 1467116498@qq.com
 * @LastEditTime: 2022-11-12 12:24:23
 * @FilePath: /oslab/lab0/os22fal-stu/src/lab1/init/main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "printk.h"
#include "sbi.h"
#include "proc.h"

extern void test();

int start_kernel() {
    test(); // DO NOT DELETE !!!

	return 0;
}
