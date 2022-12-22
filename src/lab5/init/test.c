/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-10-23 13:22:21
 * @LastEditors: Theta 1467116498@qq.com
 * @LastEditTime: 2022-11-12 11:23:58
 * @FilePath: /lab2/init/test.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "printk.h"
#include "defs.h"

// Please do not modify

void test() {
    int i = 0;
    while (1)
    {
        if (++i % 100000000 == 0)
        {
            i = 0;
            //printk("kernel is running!\n");
        }
    }
}
