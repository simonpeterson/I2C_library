.include "xc.inc"
.text ;BP (put the following data in ROM(program memory))
; This is a library, thus it can *not* contain a _main function: the C file will
; deine main(). However, we
; we will need a .global statement to make available ASM functions to C code.
; All functions utilized outside of this file will need to have a leading
; underscore (_) and be included in a comment delimited list below.
.global _step_delay
_step_delay:
    ;2 cycles to call
    repeat #15993 ;1 cycle to call total 3
    nop ;total = 15994 + 3 = 15997
    return ;3 cycles, 16000 seconds


