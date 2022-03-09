
#define CLEAROS_DEV
#define OSDEV_COMPATIBILITY

#include "os/vga.h"
#include "os/asm.h"
#include "os/timer.hpp"
#include "os/c21/vector.h"
int main(int argc, char** argv) {
    srand_init();
    std::ClearScreenColour();
    std::disableCursor();
    std::ClearLineColour(0, '#' | TerminalAttributes::BackgroundDarkGray | TerminalAttributes::ForegroundBrightMagenta);
    std::prnlstr("ClearOS v0.0.1", 0, TerminalAttributes::BackgroundDarkGray);
    while (true) {
        tick();
        std::prni(time.ticks_per_ms, std::loc(16,4));
        std::prni(rand() % 10, std::loc(16, 5));
    }
    return 0;
}
