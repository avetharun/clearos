#ifndef __avetharun_clearos_rt_os_cstd_vga_h
#define __avetharun_clearos_rt_os_cstd_vga_h
#include "math.h"
#include "types.h"
#include "stdlib.h"
#include "asm.h"
#if !defined(KERNEL_VMA)
	#define KERNEL_VMA   0xffffffff80000000 
#endif // KERNEL_VMA

//+-----------+--------------+--------------+---------------+
//| Blink (1) | BG Color (3) | FG Color (4) | Character (8) |
//+-----------+--------------+--------------+---------------+
enum TerminalAttributes : __u16 {
	BackgroundBlack         = 0x0000,
	BackgroundBlue          = 0x1000,
	BackgroundGreen         = 0x2000,
	BackgroundCyan          = 0x3000,
	BackgroundRed           = 0x4000,
	BackgroundMagenta       = 0x5000,
	BackgroundBrown         = 0x6000,
	BackgroundGray          = 0x7000,
    BackgroundDarkGray      = 0x8000,
	BackgroundBrightBlue	= 0x9000,
	BackgroundBrightGreen	= 0xa000,
	BackgroundBrightCyan	= 0xb000,
	BackgroundBrightRed 	= 0xc000,
	BackgroundBrightMagenta = 0xd000,
	BackgroundYellow        = 0xe000,
	BackgroundWhite	        = 0xf000,
    

	ForegroundBlack         = 0x0000,
	ForegroundBlue          = 0x0100,
	ForegroundGreen         = 0x0200,
	ForegroundCyan          = 0x0300,
	ForegroundRed           = 0x0400,
	ForegroundMagenta       = 0x0500,
	ForegroundBrown         = 0x0600,
	ForegroundGray          = 0x0700,

	ForegroundDarkGray      = 0x0800,
	ForegroundBrightBlue	= 0x0900,
	ForegroundBrightGreen	= 0x0a00,
	ForegroundBrightCyan	= 0x0b00,
	ForegroundBrightRed 	= 0x0c00,
	ForegroundBrightMagenta = 0x0d00,
	ForegroundYellow        = 0x0e00,
	ForegroundWhite	        = 0x0f00,
};

namespace std
{


    // Effectively a pointer to the VGA buffer
    const __u64  D_TextBufferStart  = 0xb8000;

    const __u16  D_TextWidth  = 80;
    const __u16  D_TextHeight = 26;
    const __u16  D_TextSize   = D_TextWidth * D_TextHeight;
          __u16  D_ClearColor = (BackgroundBlack | ForegroundWhite);
          auto   D_TextBuf = reinterpret_cast<__u16 *>(D_TextBufferStart);
          __u16  getClearColour() { return D_ClearColor; }
    // VGA buffer starting offset
    uintptr_t VGA_OFFSET = D_TextBufferStart;
    
    void disableCursor() {
        outb(0x3D4, 0x0A);
        outb(0x3D5, 0x20);
        return;
    }
    void setCursorPosition(int x, int y) {
        uint16 pos = y * D_TextWidth + x;
    
        outb(0x3D4, 0x0F);
        outb(0x3D5, (uint8) (pos & 0xFF));
        outb(0x3D4, 0x0E);
        outb(0x3D5, (uint8) ((pos >> 8) & 0xFF));
    }
    uint16 getCursorPosition(char * x = nullptr, char * y = nullptr)
    {
        uint16 pos = 0;
        outb(0x3D4, 0x0F);
        pos |= inb(0x3D5);
        outb(0x3D4, 0x0E);
        pos |= ((uint16)inb(0x3D5)) << 8;
        if (x != nullptr) {
            *x = (pos);
        }
        if (y != nullptr) {
            *y = (pos >> 8);
        }

        return pos;
    }

    void enableCursor(uint8 cursor_start = 0, uint8 cursor_end = 0, uint16 _cursor_internal = 1)
    {
        if (cursor_start == 0 && cursor_end == 0) {
            _cursor_internal = getCursorPosition();
            cursor_end = (_cursor_internal >> 8) & 0xFF;
            cursor_end = (_cursor_internal) & 0xFF;
        }
        outb(0x3D4, 0x0A);
        outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
    
        outb(0x3D4, 0x0B);
        outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
    }


    int loc(int x, int y) {
        return y * D_TextWidth + x;
    }
    void vga_loc(int loc, int * x, int * y) {
        if (x != nullptr){
            *x = loc % D_TextWidth;
        }
        if (y != nullptr){
            *y = loc / D_TextWidth;
        }
    }


    void __FormatStringClearColour(char* & string, __u16 attribute) {
        while ((*string++ | attribute) != '\0');
    }
    void ClearScreenColour(__u16 attribute = 0) {
        attribute = (attribute == 0) ? getClearColour() : attribute;
        for(int i = 0; i < D_TextSize; ++i) {
            D_TextBuf[i] = attribute;
        }
    }
    void ClearCharColour(int at, __u16 attribute = 0) {
        attribute = (attribute == 0) ? getClearColour() : attribute;
        D_TextBuf[at] = attribute;
    }
    void ClearLineColour(int y, __u16 attribute = 0) {
        attribute = (attribute == 0) ? getClearColour() : attribute;
        for (int x = 0; x < D_TextWidth; x++) {
            ClearCharColour(loc(x,y), attribute);
        }
    }
    void ClearColumnColour(int x, __u16 attribute = 0) {
        attribute = (attribute == 0) ? getClearColour() : attribute;
        for (int y = 0; y < D_TextWidth; y++) {
            ClearCharColour(loc(x,y), attribute);
        }
    }
    // Shorthand for re-defining D_ClearColor
    void SetClearColor(__u16 attribute) {
        D_ClearColor = attribute;
    }



    // Print N bytes of src into VGA buffer
    void prnnstr(const char* src, int at, size_t n, __u16 attribute = 0) {
        attribute = (attribute == 0) ? getClearColour() : attribute;
        int offset_ = at;
        for (int i = 0; i < n; i++) {
            char _c = src[i];
            switch (_c) {
                case '\n' : {
                    int _y;
                    vga_loc(offset_, nullptr, &_y);
                    offset_ = loc(0, _y+1);
                    continue;
                } break;
                // Leave the next character as-is
                case '\216': {
                    offset_++;
                    continue;
                }
            }
            D_TextBuf[offset_] = src[i] | attribute;
            offset_++;
        }
    }
    void prnlnstr(const char* src, int y, size_t size, __u16 attribute = 0) {
        attribute = (attribute == 0) ? getClearColour() : attribute;
        // avoid dbz error
        if (size == 0) {return;}
        int offset_half = size / 2;
        int vga_half = D_TextWidth / 2;
        int at = loc(vga_half - offset_half, y);
        prnnstr(src, at, size);
    }
    // Print a centered line of null-terminated characters into the VGA buffer.
    void prnlstr(const char* src, int y, __u16 attribute = 0) {
        attribute = (attribute == 0) ? getClearColour() : attribute;
        int size = __find_null_terminator_in_arr(src);
        prnlnstr(src, y, size, attribute);
    }
    // Prints null-terminated string into the VGA buffer
    void prnstr(const char* src, int at, __u16 attribute = 0) {
        attribute = (attribute == 0) ? getClearColour() : attribute;
        int n_t_offset = __find_null_terminator_in_arr(src);
        prnnstr(src, at, n_t_offset);
    }
    // Print unsigned char into VGA buffer 
    void prnuc(unsigned char s, int at, __u16 attribute = 0) {
        attribute = (attribute == 0) ? getClearColour() : attribute;
        D_TextBuf[at] = s | D_ClearColor;
    }

    // Print (int) to VGA buffer
    int prni(signed int num, int at, __u16 attribute = 0) {
        attribute = (attribute == 0) ? getClearColour() : attribute;
        int offset = at;
        int amt_digits = digitsInNum(num, 10);
        if (num < 0) {
            // Negative number, draw a '-' before numbers.
            prnuc('-', offset);
            amt_digits += 1;
        }
        
        char* digits = new char[amt_digits];
        int i = amt_digits - (num < 0) ? 1 : 0;
        offset += amt_digits;
        while( num != 0 )
        {
            const int least_significant_digit = num % 10 ;
            digits[i] = (unsigned char)( least_significant_digit + 48 ); 
            num /= 10;
            prnuc(digits[i], offset);
            i--;

            offset -= 1;
        }
        free(digits);
        // return the size of the number (+ the negative, if applicable)
        return amt_digits;
    }
    int prnli(signed int num, int y, __u16 attribute = 0) {
        attribute = (attribute == 0) ? getClearColour() : attribute;
        int size = digitsInNum(num);
        while (true) {}
        if (size == 0) {return 0;}
        int offset_half = size / 2;
        int vga_half = D_TextWidth / 2;
        int at = loc(vga_half - offset_half, y);
        prni(num, at, attribute);
    }

} // namespace std

#endif // __avetharun_clearos_rt_os_cstd_vga_h