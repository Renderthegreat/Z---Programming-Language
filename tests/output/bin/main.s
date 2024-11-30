# 0 "tests/output//main.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "tests/output//main.S"
# 1 "tests/output//zs.h" 1


.data
    MAP = 0x000000000
# 61 "tests/output//zs.h"
.macro NOT_IMPLEMENTED SOURCE
    .print "[0;35mNot Implemented yet[0m!"
    .print "[0;34mSource: [0m"
    .print "\SOURCE"
.endm



.macro FIRST_BYTE REGISTER




.endm


.macro ZS ACTION
    .ifc \ACTION, INIT
        .print "[1;31mZ#[0m Compiling File"
        FIRST_BYTE x0

    .endif
    .ifc \ACTION, END
        .print "[1;32mZ#[0;32m Compilation Complete[0m!"

    .endif



.endm

.macro POOL ACTION, V, VI
    .ifc \ACTION, CREATE


    .endif
    .ifc \ACTION, SWAP


    .endif



.endm

.macro FUNCTION ACTION, V
    .ifc \ACTION, CALL
        NOT_IMPLEMENTED "FUNCTION CALL"

    .endif
    .ifc \ACTION, LOCAL
        NOT_IMPLEMENTED "FUNCTION LOCAL"

    .endif



.endm
# 2 "tests/output//main.S" 2
.bss
.data
 t_size_void = 0x00000000
 t_size_byte = 0x00000001
.text
.global Main

Main:
 POOL CREATE
.global _start
_start:
 ZS INIT
 FUNCTION CALL "Main"
 ZS END
