

global out8, in8

section .bss

char1: resb 1;
char2: resb 1;
char3: resb 1;
char4: resb 1;
short1: resb 2;
short2: resb 2;
short3: resb 2;
short4: resb 2;
    
; end section .bss
; char/bool         | AL
; short             | AX
; int               | EAX
; long              | RAX


section .text