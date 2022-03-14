#define CLEAROS_DEV
#define OSDEV_COMPATIBILITY


#include "os/vga.h"
#include "os/asm.h"
#include "os/int/idt.h"
#include "os/int/isr.h"
#include "os/timer.hpp"
#include "os/c21/vector.h"
int main(int argc, char** argv) {
    std::ClearScreenColour();
    tick();
    std::disableCursor();
    std::ClearLineColour(0, '#' | TerminalAttributes::BackgroundDarkGray | TerminalAttributes::ForegroundBrightMagenta);
    std::ClearLineColour(std::D_TextHeight -1, '#' | TerminalAttributes::BackgroundDarkGray | TerminalAttributes::ForegroundBrightMagenta);
    std::prnlstr("ClearOS v0.0.1a", 0, TerminalAttributes::BackgroundCyan);

    
    srand_init();

    while (true) {
        tick();
    }
    return 0;
}
