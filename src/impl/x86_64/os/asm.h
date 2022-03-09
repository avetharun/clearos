#if !defined(__clearos_cstd_asm_h__)
#define __clearos_cstd_asm_h__


#include "alloc.h"
#include "types.h"
// Thank you, osdev wiki.



struct INT16REGS {
    unsigned int    ax, bx, cx, dx, si, di, cflag, flags;
};

struct INT8REGS {
    unsigned char   al, ah, bl, bh, cl, ch, dl, dh;
};

union   REGS    {
    struct  INT16REGS x;
    struct  INT8REGS h;
};

struct  SREGS   {
    unsigned int    es;
    unsigned int    cs;
    unsigned int    ss;
    unsigned int    ds;
};
inline void push(int register_) {
    asm volatile("push %0" : : ""(register_));
}
inline void pop(int register_) {
    asm volatile("pop %0" : : ""(register_));
}
// Collection of utility assembly functions, because `asm volatile ( "" )` is nasty looking inline.
inline void outb( unsigned short port, unsigned char val ) {
   asm volatile("outb %0, %1" : : "a"(val), "Nd"(port) );
}
inline uint8 inb(uint16 port) {
    uint8 ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

inline void out32( unsigned short port, uint32 val ) {
   asm volatile("out %0, %1" : : "a"(val), "Nd"(port) );
}
inline uint32 in32(uint16 port) {
    uint32 ret;
    asm volatile ( "in %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}


inline void movl(int source, int dest) {
   asm volatile("movl %0, %1" : : ""(source), ""(dest) );
}
inline void mov(int source, int dest) {
    asm volatile("mov %0, %1" : : ""(source), ""(dest));
}
// todo: read a book on assembly + C
inline void int86(int code, union REGS * input_regs, union REGS * output_regs) {
    
}
inline void intrt(int code) {
    asm volatile("int %0" : : "Nd"(code));
}



// Wait ~10uS
inline void io_wait(void)
{
    outb(0x80, 0);
}

// Assume we're in 64 bit mode


inline uint64 rdtsc()
{
    uint64 ret;
    asm volatile ( "rdtsc" : "=A"(ret) );
    return ret;
}

inline unsigned long read_cr0(void)
{
    unsigned long val;
    asm volatile ( "mov %%cr0, %0" : "=r"(val) );
    return val;
}

inline bool are_interrupts_enabled()
{
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}
inline void sti() {
    asm volatile("sti");
}
inline void cli() {
    asm volatile("cli");
}

void NMI_enable() {
    outb(0x70, inb(0x70) & 0x7F);
    inb(0x71);
}

void NMI_disable() {
    outb(0x70, inb(0x70) | 0x80);
    inb(0x71);
}


unsigned read_pit_count(void) {
	unsigned count = 0;
 
	// Disable interrupts
	cli();
 
	// al = channel in bits 6 and 7, remaining bits clear
	outb(0x43,0b0000000);
 
	count = inb(0x40);		// Low byte
	count |= inb(0x40)<<8;		// High byte
 
	return count;
}

extern void enter_v86(uint32_t ss, uint32_t esp, uint32_t cs, uint32_t eip);


#endif // __clearos_cstd_asm_h__

