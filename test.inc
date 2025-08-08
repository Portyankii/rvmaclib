.section .rodata
red:     .asciz "\033[31m"
green:   .asciz "\033[32m"
blue:    .asciz "\033[34m"
reset:   .asciz "\033[0m"

.macro colorwrite str, color, len
    # write color
    .ifc \color, red
        syswrite red, 5
    .else
    .ifc \color, green
        syswrite green, 5
    .else
    .ifc \color, blue
        syswrite blue, 5
    .else
        .error "Unknown color"
    .endif
    .endif
    .endif
    
    # write string
    syswrite \str, \len
    
    # write reset
    syswrite reset, 4
.endm
# These macros simplify common syscall operations

# syscall value constants
.equ SYS_READ,     63
.equ SYS_WRITE,    64
.equ SYS_OPEN,     56
.equ SYS_CLOSE,    57
.equ SYS_EXIT,     93
.equ SYS_BRK,      214
.equ SYS_MMAP,     222

# File descriptors
.equ STDIN,  0
.equ STDOUT, 1  
.equ STDERR, 2

# Write to stdout - usage: syswrite buffer, length
.macro syswrite buffer, length
    li a7, SYS_WRITE        # syscall number for write
    li a0, STDOUT           # file descriptor (stdout)
    la a1, \buffer          # buffer address
    li a2, \length          # number of bytes to write
    ecall                   # make syscall
.endm

# Write to stderr - usage: syserrwrite buffer, length  
.macro scrwrite buffer, length
    li a7, SYS_WRITE        # syscall number for write
    li a0, STDERR           # file descriptor (stderr) 
    la a1, \buffer          # buffer address
    li a2, \length          # number of bytes to write
    ecall                   # make syscall
.endm

# Write to specific fd - usage: syswritefd fd, buffer, length
.macro writefd fd, buffer, length
    li a7, SYS_WRITE        # syscall number for write
    li a0, \fd              # file descriptor
    la a1, \buffer          # buffer address  
    li a2, \length          # number of bytes to write
    ecall                   # make syscall
.endm

# Read from stdin - usage: sysread buffer, maxlen
.macro readstdin buffer, maxlen
    li a7, SYS_READ         # syscall number for read
    li a0, STDIN            # file descriptor (stdin)
    la a1, \buffer          # buffer address
    li a2, \maxlen          # maximum bytes to read
    ecall                   # make syscall
.endm

# Read from specific fd - usage: sysreadfd fd, buffer, maxlen
.macro readfd fd, buffer, maxlen
    li a7, SYS_READ         # syscall number for read
    li a0, \fd              # file descriptor
    la a1, \buffer          # buffer address
    li a2, \maxlen          # maximum bytes to read  
    ecall                   # make syscall
.endm

# Exit program - usage: sysexit exitcode
.macro sysexit exitcode
    li a7, SYS_EXIT         # syscall number for exit
    li a0, \exitcode        # exit status
    ecall                   # make syscall
.endm

# Open file - usage: sysopen filename, flags, mode
# Returns fd in a0, or negative on error
.macro sysopen filename, flags, mode
    li a7, SYS_OPEN         # syscall number for open
    la a0, \filename        # filename address
    li a1, \flags           # flags (O_RDONLY=0, O_WRONLY=1, O_RDWR=2)
    li a2, \mode            # mode (permissions)
    ecall                   # make syscall
.endm

# Close file - usage: sysclose fd
.macro sysclose fd
    li a7, SYS_CLOSE        # syscall number for close
    li a0, \fd              # file descriptor
    ecall                   # make syscall  
.endm

# Print newline - usage: writenewline
.macro writenewline
    li a7, SYS_WRITE        # syscall number for write
    li a0, STDOUT           # stdout
    la a1, newline_str      # newline string
    li a2, 1                # length
    ecall                   # make syscall
.endm
