#define nullptr (void*)0x0
#define true 1
#define false 0
#define bool _Bool
#define CLEAROS_DEV
#define OSDEV_COMPATIBILITY
#include "../x86_64/os/alloc.h"
#include "../x86_64/os/int/idt.h"
#include "../x86_64/os/stdbool.h"
static uintptr_t * __internal_data_seg_end = (uintptr_t * )0xffffffff;
static uintptr_t * __internal_brk = 0;
static int __internal_page_size = 32; // bytes
extern int main(int argc, char** argv);

void kernel_main() {
    asm("__kernel_start:\n");
    main(0, nullptr);
}
