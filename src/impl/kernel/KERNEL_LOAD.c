#define nullptr (void*)0x0
#define true 1
#define false 0
#define CLEAROS_DEV
#define OSDEV_COMPATIBILITY

extern int main(int argc, char** argv);

void kernel_main() {
    main(0, nullptr);
    asm("hlt");
}
