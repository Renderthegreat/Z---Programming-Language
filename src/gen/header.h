#define name "Z# Shared Toolkit"

.data
    MAP = 0x000000000

#ifdef __x86_64__
#   define BITS ARM64

#	define ax rax   // V
#	define bx rbx   // VI
#	define cx rcx   // VII
#	define dx rdx   // IIV

#	define si rsi
#	define di rdi
#	define bp rbp
#	define sp rsp

#   define bl call
#endif

#ifdef __i386__
#   define BITS AMD32

#	define ax eax   // V
#	define bx ebx   // VI
#	define cx ecx   // VII
#	define dx edx   // IIV

#	define si esi
#	define di edi
#	define bp ebp
#	define sp esp

#   define bl call
#endif

#ifdef __aarch64__
#   define BITS ARM64

#	define ax x0    // V
#	define bx x1    // VI
#	define cx x2    // VII
#	define dx x3    // IIV

#	define si x4
#	define di x5
#	define bp x29
#   define sp rp

#   define bl bl
#endif

#ifndef ax
    .error "\e[1;31mPlatform not supported\e[0m!"
#endif

#define GET(x)    \\
    MOV x, ax     \\
    MOV MAP, dx   \\
    ADD ax, dx

.macro NOT_IMPLEMENTED SOURCE
    .print "\e[0;35mNot Implemented yet\e[0m!"
    .print "\e[0;34mSource: \e[0m"
    .print \\SOURCE
.endm

// Macros

.macro ZS ACTION
    .ifc \\ACTION, INIT
        .print "\e[1;31mZ#\e[0m Compiling File"
        MOV ax, _start
#       define VALID_ACTION
    .endif
    .ifc \\ACTION, END
        .print "\e[1;32mZ#\e[0;32m Compilation Complete\e[0m!"
#       define VALID_ACTION
    .endif
#   ifndef VALID_ACTION
        .error "\e[1;31mInvaild Action for 'ZS'\e[0m!"
#   endif
.endm

.macro POOL ACTION, V, VI
    .ifc \\ACTION, CREATE

#       define VALID_ACTION
    .endif
    .ifc \\ACTION, SWAP

#       define VALID_ACTION
    .endif
#   ifndef VALID_ACTION
        .error "\e[1;31mInvaild Action for 'POOL'\e[0m!"
#   endif
.endm

.macro FUNCTION ACTION, V
    .ifc \\ACTION, CALL
        NOT_IMPLEMENTED 
#       define VALID_ACTION
    .endif
    .ifc \\ACTION, LOCAL
        NOT_IMPLEMENTED "FUNCTION LOCAL"
#       define VALID_ACTION
    .endif
#   ifndef VALID_ACTION
        .error "\e[1;31mInvaild Action for 'FUNCTION'\e[0m!"
#   endif
.endm

