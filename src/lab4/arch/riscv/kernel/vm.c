// arch/riscv/kernel/vm.c

#include "defs.h"
#include "types.h"
#include "string.h"
#include "printk.h"

/* early_pgtbl: 用于 setup_vm 进行 1GB 的 映射。 */
unsigned long  early_pgtbl[512] __attribute__((__aligned__(0x1000)));

extern char _srodata[];
extern char _stext[];
extern char _sdata[];
extern char _sbss[];
extern char _ekernel[];

// 0x0000000080200000,物理地址下的_start
void setup_vm(void) {
    /* 
    1. 由于是进行 1GB 的映射 这里不需要使用多级页表 
    2. 将 va 的 64bit 作为如下划分： | high bit | 9 bit | 30 bit |
        high bit 可以忽略dfd
        中间9 bit 作为 early_pgtbl 的 index
        低 30 bit 作为 页内偏移 这里注意到 30 = 9 + 9 + 12， 即我们只使用根页表， 根页表的每个 entry 都对应 1GB 的区域。 
    3. Page Table Entry 的权限 V | R | W | X 位设置为 1
    */

   uint64 VPN_2 = (PHY_START & 0x0000007FC0000000) >> 30; // PA = VA
   
   early_pgtbl[VPN_2] = PHY_START >> 2 | 0xF; // 设置protection bits

   uint64 VPN_2_HIGH = ((PHY_START + PA2VA_OFFSET) & 0x0000007FC0000000) >> 30; // PA + PV2VA_OFFSET == VA

    early_pgtbl[VPN_2_HIGH] = PHY_START >> 2 | 0xF; // 设置protection bits

    return;
}

/* swapper_pg_dir: kernel pagetable 根目录， 在 setup_vm_final 进行映射。 */
unsigned long  swapper_pg_dir[512] __attribute__((__aligned__(0x1000)));

void setup_vm_final(void) {
    memset(swapper_pg_dir, 0x0, PGSIZE);
    printk("setup_vm_final\n");
    printk("_stext = %lx\n", (uint64)_stext);
    printk("_srodata = %lx\n", (uint64)_srodata);
    printk("_sdata = %lx\n", (uint64)_sdata);
    printk("_sbss = %lx\n", (uint64)_sbss);
    printk("_ekernel = %lx\n", (uint64)_ekernel);
    
    // No OpenSBI mapping required

    // mapping kernel text X|-|R|V
    printk("create_mapping\n");
    create_mapping(swapper_pg_dir, VM_START+OPENSBI_SIZE,(uint64)PHY_START + OPENSBI_SIZE,(uint64)_srodata - (uint64)_stext, 11);

    // mapping kernel rodata -|-|R|V  
    create_mapping(swapper_pg_dir, VM_START + OPENSBI_SIZE + (uint64)_srodata - (uint64)_stext, PHY_START + OPENSBI_SIZE + (uint64)_srodata - (uint64)_stext, (uint64)_sdata - (uint64)_srodata, 3);

    // mapping other memory -|W|R|V
    create_mapping(swapper_pg_dir, VM_START + OPENSBI_SIZE + (uint64)_sdata - (uint64)_stext, PHY_START + OPENSBI_SIZE + (uint64)_sdata - (uint64)_stext, PHY_SIZE - OPENSBI_SIZE - (uint64)_sdata + (uint64)_stext, 7);
    printk("create_mapping end\n");

    // set satp with swapper_pg_dir

    printk("swapper_pg_dir = %lx\n", (uint64)swapper_pg_dir);
    uint64 val = (uint64)swapper_pg_dir - PA2VA_OFFSET; // pa = va - PA2VA_OFFSET
    val = val >> 12; // ppn = pa >> 12
    val = val | 0x8000000000000000;    
    printk("val = %lx\n", val);

    __asm__ volatile (
        "csrw satp, %0\n"
        "sfence.vma zero, zero\n"
        "fence.i\n"
        :
        :"r"(val)
    ); 

    // flush TLB
    // asm volatile("sfence.vma zero, zero");
  
    // flush icache
    // asm volatile("fence.i");
    return;
}


/* 创建多级页表映射关系 */
create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm) {
    /*
    pgtbl 为根页表的基地址
    va, pa 为需要映射的虚拟地址、物理地址
    sz 为映射的大小
    perm 为映射的读写权限

    创建多级页表的时候可以使用 kalloc() 来获取一页作为页表目录
    可以使用 V bit 来判断页表项是否存在
    */
   printk("pgtbl = %lx, va = %lx, pa = %lx, sz = %lx, perm = %d\n", pgtbl, va, pa, sz, perm);
   int page_num = (page_num % 0x1000) ? ((sz >> 12) + 1) : (sz >> 12);
   for(int i = 0; i < page_num; i++) {
        uint64 addr_va = va + i * PGSIZE;
        uint64 addr_pa = pa + i * PGSIZE;

        uint64 vpn2 = (addr_va >> 30) & 0x1FF;
        uint64 vpn1 = (addr_va >> 21) & 0x1FF;
        uint64 vpn0 = (addr_va >> 12) & 0x1FF;

        uint64 fpte = pgtbl[vpn2];
        
        if(!(fpte & 0x1)) {
            uint64 content = kalloc() - PA2VA_OFFSET;
            fpte = (content & 0x00FFFFFFFFFFF000) >> 2 | 1;
            pgtbl[vpn2] =  fpte;
        }

        uint64* pgtbl_2;
        pgtbl_2 = (uint64*)(((fpte & 0x003FFFFFFFFFFFC00) << 2) + PA2VA_OFFSET);
        uint64 spte = pgtbl_2[vpn1];
        if(!(spte & 0x1)) { 
            uint64 content = kalloc() - PA2VA_OFFSET;
            spte = (content & 0x00FFFFFFFFFFF000) >> 2 | 1;
            pgtbl_2[vpn1] = spte;
        }
        
        uint64* pgtbl_3;
        pgtbl_3 = (uint64*)(((spte & 0x003FFFFFFFFFFFC00) << 2) + PA2VA_OFFSET);
        uint64 tpte = ((addr_pa & 0x00FFFFFFFFFFF000) >> 2) | perm;
        pgtbl_3[vpn0] = tpte;
   }
}