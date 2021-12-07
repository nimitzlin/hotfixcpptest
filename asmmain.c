#include <cassert>
#include <stdint.h>
#include <iostream>
#include <sys/mman.h>

__attribute__ ((ms_hook_prologue))
__attribute__ ((aligned(8)))
__attribute__ ((noinline))
__attribute__ ((noclone))
void func1()
{
    std::cout<< "hello111" << std::endl;

}
void func2()
{
    std::cout<< "hello222" << std::endl;

}

void
hotpatch(void *target, void *replacement)
{
    assert(((uintptr_t)target & 0x07) == 0); // 8-byte aligned?
    void *page = (void *)((uintptr_t)target & ~0xfff);
    mprotect(page, 4096, PROT_WRITE | PROT_EXEC);
    uint32_t rel = (char *)replacement - (char *)target - 5;
    union {
            uint8_t bytes[8];
            uint64_t value;
    } instruction = { {0xe9, rel >> 0, rel >> 8, rel >> 16, rel >> 24} };
    *(uint64_t *)target = instruction.value;
    mprotect(page, 4096, PROT_EXEC);
}


int main()
{
    func1();
    hotpatch(&func1, &func2);
    std::cout << "after repalce" << std::endl;
    func1();

}
