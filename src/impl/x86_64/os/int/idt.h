#if !defined(KERNEL_INTERRUPT_IDT_H_CLEAROS)
#define KERNEL_INTERRUPT_IDT_H_CLEAROS
#ifndef __cplusplus 
#define __C_COMPILED
#endif
#include "../types.h"
#include "../alloc.h"
/* Defines an IDT entry */
struct idt_entry {
   unsigned short base_lo;
   unsigned short sel;        /* Our kernel segment goes here! */
   unsigned char always0;     /* This will ALWAYS be set to 0! */
   unsigned char flags;       /* Set using the above table! */
   unsigned short base_hi;
};
struct idt_ptr {
   unsigned short limit;
   void* base;
};

struct idt_entry idt[256];
struct idt_ptr idtp;
/* Use this function to set an entry in the IDT. Alot simpler
*  than twiddling with the GDT ;) */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
   /* The interrupt routine's base address */
   idt[num].base_lo = (base & 0xFFFF);
   idt[num].base_hi = (base >> 16) & 0xFFFF;

   /* The segment or 'selector' that this IDT entry will use
   *  is set here, along with any access flags */
   idt[num].sel = sel;
   idt[num].always0 = 0;
   idt[num].flags = flags;
}

void idt_load();
/* Installs the IDT */
void idt_install() {
   memset(&idt, 0, sizeof(struct idt_entry) * 256);
   /* Sets the special IDT pointer up, just like in 'gdt.c' */
   idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
   idtp.base = &idt;
   /* Clear out the entire IDT, initializing it to zeros */

   /* Add any new ISRs to the IDT here using idt_set_gate */

   /* Points the processor's internal register to the new IDT */
   idt_load();
}

/* This is a very repetitive function... it's not hard, it's
*  just annoying. As you can see, we set the first 32 entries
*  in the IDT to the first 32 ISRs. We can't use a for loop
*  for this, because there is no way to get the function names
*  that correspond to that given entry. We set the access
*  flags to 0x8E. This means that the entry is present, is
*  running in ring 0 (kernel level), and has the lower 5 bits
*  set to the required '14', which is represented by 'E' in
*  hex. */
/* This defines what the stack looks like after an ISR was running */
struct isr_regs
{
   unsigned int gs, fs, es, ds;      /* pushed the segs last */
   unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
   unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
   unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

#ifdef __cplusplus
extern "C" {
#endif
   /* These are function prototypes for all of the exception
   *  handlers: The first 32 entries in the IDT are reserved
   *  by Intel, and are designed to service exceptions! */
   void _isr0();
   void _isr1();
   void _isr2();
   void _isr3();
   void _isr4();
   void _isr5();
   void _isr6();
   void _isr7();
   void _isr8();
   void _isr9();
   void _isr10();
   void _isr11();
   void _isr12();
   void _isr13();
   void _isr14();
   void _isr15();
   void _isr16();
   void _isr17();
   void _isr18();
   void _isr19();
   void _isr20();
   void _isr21();
   void _isr22();
   void _isr23();
   void _isr24();
   void _isr25();
   void _isr26();
   void _isr27();
   void _isr28();
   void _isr29();
   void _isr30();
   void _isr31();
#ifdef __cplusplus
}
#endif


void isrs_install()
{
    idt_set_gate(0, (unsigned)_isr0, 0x08, 0x8E);
    idt_set_gate(1, (unsigned)_isr1, 0x08, 0x8E);
    idt_set_gate(2, (unsigned)_isr2, 0x08, 0x8E);
    idt_set_gate(3, (unsigned)_isr3, 0x08, 0x8E);
    idt_set_gate(4, (unsigned)_isr4, 0x08, 0x8E);
    idt_set_gate(5, (unsigned)_isr5, 0x08, 0x8E);
    idt_set_gate(6, (unsigned)_isr6, 0x08, 0x8E);
    idt_set_gate(7, (unsigned)_isr7, 0x08, 0x8E);

    idt_set_gate(8, (unsigned)_isr8, 0x08, 0x8E);
    idt_set_gate(9, (unsigned)_isr9, 0x08, 0x8E);
    idt_set_gate(10, (unsigned)_isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned)_isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned)_isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned)_isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned)_isr14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned)_isr15, 0x08, 0x8E);

    idt_set_gate(16, (unsigned)_isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned)_isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned)_isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned)_isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned)_isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned)_isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned)_isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned)_isr23, 0x08, 0x8E);

    idt_set_gate(24, (unsigned)_isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned)_isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned)_isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned)_isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned)_isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned)_isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned)_isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned)_isr31, 0x08, 0x8E);
}

#endif