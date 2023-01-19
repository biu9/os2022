/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-09-20 17:12:36
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-10-02 18:43:10
 * @FilePath: /lab1/lib/print.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "print.h"
#include "sbi.h"

void puts(char *s) {
    // unimplemented
    int i=0;
    while(s[i]){
        sbi_ecall(0x1,0x0,s[i],0x0,0x0,0x0,0x0,0x0);
        i++;
    }
}

void puti(int x) {
    // unimplemented
    int a,b;
    int pow = 1;
    while(10*pow < x) pow *= 10;
    a=x/pow;b=x%pow;
    while(pow!=0){
        sbi_ecall(0x1,0x0,a+'0',0x0,0x0,0x0,0x0,0x0);
        pow /= 10;
        a=b/pow;
        b=b%pow;
    }
}
