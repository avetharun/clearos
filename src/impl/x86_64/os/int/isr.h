#if !defined(KERNEL_INTERRUPT_ISR_H_CLEAROS)
#define KERNEL_INTERRUPT_ISR_H_CLEAROS
#include "idt.h"
#include "../iostream.h"

/* This is a simple string array. It contains the message that
*  corresponds to each and every exception. We get the correct
*  message by accessing like:
*  exception_message[interrupt_number] */
const char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

extern "C" {
    void __ih(isr_regs *r) {
        /* Is this a fault whose number is from 0 to 31? */
        if (r->int_no < 32)
        {
            /* Display the description for the Exception that occurred.
            *  In this tutorial, we will simply halt the system using an
            *  infinite loop */
            // std::cout.set_loc(0);
            // std::cout << "Exception: " << exception_messages[r->int_no];
        }
        asm("hlt");
    }
    void __fh(isr_regs *r)
    {
        /* Is this a fault whose number is from 0 to 31? */
        if (r->int_no < 32)
        {
            /* Display the description for the Exception that occurred.
            *  In this tutorial, we will simply halt the system using an
            *  infinite loop */
            // std::cout.set_loc(0);
            // std::cout << "Exception: " << exception_messages[r->int_no];
        }
        asm("hlt");
    }

}

#endif