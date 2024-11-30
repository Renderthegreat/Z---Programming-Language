#define name "Z# Shared Toolkit"

/*
 * NOTICE: SOFTWARE GENERATED BY THIS CODE IS NOT PROTECTED UNDER THE DMCA
 *
 * Any software, code, or program generated by the Z# programming language 
 * is explicitly and permanently exempt from the protections of the 
 * Digital Millennium Copyright Act (DMCA). 
 *
 * This means:
 * 1. **You may freely modify, adapt, or redistribute the generated code** 
 *    without fear of legal consequences under copyright law, including 
 *    DMCA takedown requests or anti-circumvention claims.
 * 
 * 2. **No one, including the original author(s) of the generated code, 
 *    may enforce restrictions that would prevent you from editing, 
 *    improving, or repurposing the software.**
 *
 * 3. **You are protected from any legal liability that may arise from 
 *    modifying or redistributing the generated software.**
 *
 * RATIONALE:
 * The Z# programming language is built with the goal of ensuring software 
 * remains fully open, modifiable, and free from legal restrictions. Any form 
 * of legal protection that prohibits modification or restricts user freedom 
 * runs counter to the principles of Z#. By generating code with Z#, the 
 * user agrees to these terms and acknowledges the freedom to modify 
 * software without legal interference.
 *
 * DISCLAIMER:
 * This header applies only to software or code generated *using* the 
 * Z# programming language. The Z# language itself and its implementation 
 * are copyrighted and protected by their respective authors unless explicitly 
 * stated otherwise.
 *
 * By using, modifying, or redistributing software created with Z#, you 
 * acknowledge and respect the right of others to do the same.
 */

.equ MAP_START, 0x20000000
.equ MAP_SIZE, 65536

.section .map, "aw", %progbits
map:
    .space MAP_SIZE

.text

// ARCHITECTURE SPECIFIC

#ifdef __x86_64__
#   define AMD64 1
#   define BITS AMD64
    .print "\e[1;31mZ#\e[0m Compiling for AMD64"

#	define ax rax   // V
#	define bx rbx   // VI
#	define cx rcx   // VII
#	define dx rdx   // IIX

#	define si rsi
#	define di rdi
#	define bp rbp
#	define sp rsp
#endif

#ifdef __i386__
#   define AMD32 1
#   define BITS AMD32
    .print "\e[1;31mZ#\e[0m Compiling for AMD32"

#	define ax eax   // V
#	define bx ebx   // VI
#	define cx ecx   // VII
#	define dx edx   // IIX

#	define si esi
#	define di edi
#	define bp ebp
#	define sp esp
#endif

#ifdef __aarch64__
#   define ARM64 1
#   define BITS ARM64
    .print "\e[1;31mZ#\e[0m Compiling for ARM64"

#	define ax x0    // V
#	define bx x1    // VI
#	define cx x2    // VII
#	define dx x3    // IIX

#	define si x4
#	define di x5
#	define bp x29
#   define sp rp
    .macro PRINT str
        ldr ax, =HELLO
        mov bx, 0

        bl printf
        ret
    .endm
#endif

#ifndef ax
    // COMPILING IS NOT SUPPORTED, ERROR OUT
    .error "\e[1;31mZ#\e[0m Platform not supported\e[0m!"
#endif

// COMPILING IS SUPPORTED

// SHORT MACROS

#define FUNCDEF(x) _FUNC_##x
#define TYPEDEF(x) _TYPE_##x

// ERRORS / WARNINGS

.set ERRORS, 1

.macro NOT_IMPLEMENTED SOURCE, ACTION
    .print "\e[1;35m██ \e[0;35mNot Implemented yet\e[0m!"
    .print "\e[1;35m██ \\SOURCE \\ACTION\e[0m"
    ZS WARNING
.endm

.macro INVALID_ACTION SOURCE, ACTION
    .print "\e[1;91m██ \e[5;91mInvalid Instruction '\\ACTION'' for '\\SOURCE''\e[0m!"
    ZS ERROR
.endm

// COMPILE
.set ERRORS, 0
.set WARNINGS, 0

.macro ERROR_SUMMARY, COUNT
    .print "\e[1;92mZ#\e[1;91m Compilation Failed with \\COUNT Error(s)\e[0m!"
.endm

.macro WARNING_SUMMARY, COUNT
    .print "\e[1;95mZ#\e[1;95m Compilation Successful With \\COUNT Warning(s)\e[0m!"
.endm

.macro SUCCESS_SUMMARY
    .print "\e[1;92mZ#\e[1;92m Compilation Successful\e[0m!"
.endm

.macro SUMMARY
    .altmacro
    .if ERRORS == 0 && WARNINGS == 0
        SUCCESS_SUMMARY
    .elseif ERRORS == 0
        WARNING_SUMMARY %WARNINGS
    .else
        ERROR_SUMMARY %ERRORS
    .endif
    .print "\e[1;96m██\e[0m 🫎  + 🐧 = ♥️"
.endm

// SYSTEMS

.macro ZS, ACTION, V
    .ifc \\ACTION, INIT
        MAP INIT
        .set VALID_ACTION, 1
    .endif
    .ifc \\ACTION, ERROR
        .set ERRORS, ERRORS + 1
        .set VALID_ACTION, 1
    .endif
    .ifc \\ACTION, WARNING
        .set WARNINGS, WARNINGS + 1
        .set VALID_ACTION, 1
    .endif
    .ifc \\ACTION, END
        SUMMARY
        .set VALID_ACTION, 1
    .endif
    .if VALID_ACTION == 0
        INVALID_ACTION "ZS", \\ACTION
    .endif

    .set VALID_ACTION, 0
.endm

.macro LOCAL, ACTION, V, VI, VII
    .ifc \\ACTION, CALL
#       if BITS == AMD64
            CALL \\V
#       endif
#       if BITS == AMD32
            CALL \\V
#       endif
#       if BITS == ARM64
            BL \\V
#       endif
        .set VALID_ACTION, 1
    .endif
    .if VALID_ACTION == 0
        INVALID_ACTION "LOCAL", \\ACTION
    .endif

    .set VALID_ACTION, 0
.endm


.macro MAP ACTION, V, VI
    .ifc \\ACTION, INIT

        .set VALID_ACTION, 1
    .endif
    .if VALID_ACTION == 0
        INVALID_ACTION "MAP", \\ACTION
    .endif
    .set VALID_ACTION, 1
.endm


.macro POOL ACTION, V, VI
    .ifc \\ACTION, CREATE

        .set VALID_ACTION, 1
    .endif
    .ifc \\ACTION, SWAP

        .set VALID_ACTION, 1
    .endif
    .if VALID_ACTION == 0
        INVALID_ACTION, "POOL", \\ACTION
    .endif
    .set VALID_ACTION, 0
.endm

.macro FUNCTION ACTION, V
    .ifc \\ACTION, CALL
        LOCAL CALL, FUNCDEF(\\V) 
        .set VALID_ACTION, 1
    .endif
    .ifc \\ACTION, LOCAL
        NOT_IMPLEMENTED "FUNCTION LOCAL"
        .set VALID_ACTION, 1
    .endif
    .if VALID_ACTION == 0
        INVALID_ACTION "FUNCTION", \\ACTION
    .endif
    .set VALID_ACTION, 0
.endm