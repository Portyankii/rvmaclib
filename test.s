.include "macros/rvmaclib.inc"

.section .data
msg: .ascii ":3 "
.section .text
.global _start


_start:
	colorwrite msg, red, 3
	colorwrite msg, green, 3
	colorwrite msg, blue, 3
	j _start
