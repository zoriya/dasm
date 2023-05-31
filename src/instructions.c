#include <stddef.h>
#include "dasm.h"

const instruction_t instructions[] = {
	{.opcode = 0x00, .name = "add", .mode = {R_M8, REG8, END}, .size = 2},
	{.opcode = 0x01, .name = "add", .mode = {R_M16, REG16, END}, .size = 3},
	{.opcode = 0x02, .name = "add", .mode = {REG8, R_M8, END}, .size = 2},
	{.opcode = 0x03, .name = "add", .mode = {REG16, R_M16, END}, .size = 3},
	{.opcode = 0x04, .name = "add al", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0x05, .name = "add ax", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0x06, .name = "push es", .mode = {END}, .size = 1},
	{.opcode = 0x07, .name = "pop es", .mode = {END}, .size = 1},
	{.opcode = 0x08, .name = "or", .mode = {R_M8, REG8, END}, .size = 2},
	{.opcode = 0x09, .name = "or", .mode = {R_M16, REG16, END}, .size = 3},
	{.opcode = 0x0A, .name = "or", .mode = {REG8, R_M8, END}, .size = 2},
	{.opcode = 0x0B, .name = "or", .mode = {REG16, R_M16, END}, .size = 3},
	{.opcode = 0x0C, .name = "or al", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0x0D, .name = "or ax", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0x0E, .name = "push cs", .mode = {END}, .size = 1},

	{.opcode = 0x10, .name = "adc", .mode = {R_M8, REG8, END}, .size = 2},
	{.opcode = 0x11, .name = "adc", .mode = {R_M16, REG16, END}, .size = 3},
	{.opcode = 0x12, .name = "adc", .mode = {REG8, R_M8, END}, .size = 2},
	{.opcode = 0x13, .name = "adc", .mode = {REG16, R_M16, END}, .size = 3},
	{.opcode = 0x14, .name = "adc al", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0x15, .name = "adc ax", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0x16, .name = "push ss", .mode = {END}, .size = 1},
	{.opcode = 0x17, .name = "pop ss", .mode = {END}, .size = 1},
	{.opcode = 0x18, .name = "sbb", .mode = {R_M8, REG8, END}, .size = 2},
	{.opcode = 0x19, .name = "sbb", .mode = {R_M16, REG16, END}, .size = 3},
	{.opcode = 0x1A, .name = "sbb", .mode = {REG8, R_M8, END}, .size = 2},
	{.opcode = 0x1B, .name = "sbb", .mode = {REG16, R_M16, END}, .size = 3},
	{.opcode = 0x1C, .name = "sbb al", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0x1D, .name = "sbb ax", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0x1E, .name = "push ds", .mode = {END}, .size = 1},
	{.opcode = 0x1F, .name = "pop ds", .mode = {END}, .size = 1},
	{.opcode = 0x20, .name = "and", .mode = {R_M8, REG8, END}, .size = 2},
	{.opcode = 0x21, .name = "and", .mode = {R_M16, REG16, END}, .size = 3},
	{.opcode = 0x22, .name = "and", .mode = {REG8, R_M8, END}, .size = 2},
	{.opcode = 0x23, .name = "and", .mode = {REG16, R_M16, END}, .size = 3},
	{.opcode = 0x24, .name = "and al", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0x25, .name = "and ax", .mode = {IMM16, END}, .size = 3},

	{.opcode = 0x3F, .name = "ass", .mode = {END}, .size = 1},
	{.opcode = 0x40, .name = "inc ax", .mode = {END}, .size = 1},
	{.opcode = 0x41, .name = "inc cx", .mode = {END}, .size = 1},
	{.opcode = 0x42, .name = "inc dx", .mode = {END}, .size = 1},
	{.opcode = 0x43, .name = "inc bx", .mode = {END}, .size = 1},
	{.opcode = 0x44, .name = "inc sp", .mode = {END}, .size = 1},
	{.opcode = 0x45, .name = "inc bp", .mode = {END}, .size = 1},
	{.opcode = 0x46, .name = "inc si", .mode = {END}, .size = 1},
	{.opcode = 0x47, .name = "inc di", .mode = {END}, .size = 1},
	{.opcode = 0x48, .name = "dec ax", .mode = {END}, .size = 1},
	{.opcode = 0x49, .name = "dec cx", .mode = {END}, .size = 1},
	{.opcode = 0x4A, .name = "dec dx", .mode = {END}, .size = 1},
	{.opcode = 0x4B, .name = "dec bx", .mode = {END}, .size = 1},
	{.opcode = 0x4C, .name = "dec sp", .mode = {END}, .size = 1},
	{.opcode = 0x4D, .name = "dec bp", .mode = {END}, .size = 1},
	{.opcode = 0x4E, .name = "dec si", .mode = {END}, .size = 1},
	{.opcode = 0x4F, .name = "dec di", .mode = {END}, .size = 1},
	{.opcode = 0x50, .name = "push ax", .mode = {END}, .size = 1},
	{.opcode = 0x51, .name = "push cx", .mode = {END}, .size = 1},
	{.opcode = 0x52, .name = "push dx", .mode = {END}, .size = 1},
	{.opcode = 0x53, .name = "push bx", .mode = {END}, .size = 1},
	{.opcode = 0x54, .name = "push sp", .mode = {END}, .size = 1},
	{.opcode = 0x55, .name = "push bp", .mode = {END}, .size = 1},
	{.opcode = 0x56, .name = "push si", .mode = {END}, .size = 1},
	{.opcode = 0x57, .name = "push di", .mode = {END}, .size = 1},
	{.opcode = 0x58, .name = "pop ax", .mode = {END}, .size = 1},
	{.opcode = 0x59, .name = "pop cx", .mode = {END}, .size = 1},
	{.opcode = 0x5A, .name = "pop dx", .mode = {END}, .size = 1},
	{.opcode = 0x5B, .name = "pop bx", .mode = {END}, .size = 1},
	{.opcode = 0x5C, .name = "pop sp", .mode = {END}, .size = 1},
	{.opcode = 0x5D, .name = "pop bp", .mode = {END}, .size = 1},
	{.opcode = 0x5E, .name = "pop si", .mode = {END}, .size = 1},
	{.opcode = 0x5F, .name = "pop di", .mode = {END}, .size = 1},
	{.opcode = 0x70, .name = "jo", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x71, .name = "jno", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x72, .name = "jc", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x73, .name = "jnc", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x74, .name = "jz", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x75, .name = "jnz", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x76, .name = "jna", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x77, .name = "ja", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x78, .name = "js", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x79, .name = "jns", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x7A, .name = "jpe", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x7B, .name = "jpo", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x7C, .name = "jl", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x7D, .name = "jnl", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x7E, .name = "jng", .mode = {REL8, END}, .size = 2},
	{.opcode = 0x7F, .name = "jg", .mode = {REL8, END}, .size = 2},

	{.opcode = 0x90, .name = "xchg ax,ax", .mode = {END}, .size = 1},
	{.opcode = 0x91, .name = "xchg cx,ax", .mode = {END}, .size = 1},
	{.opcode = 0x92, .name = "xchg dx,ax", .mode = {END}, .size = 1},
	{.opcode = 0x93, .name = "xchg bx,ax", .mode = {END}, .size = 1},
	{.opcode = 0x94, .name = "xchg sp,ax", .mode = {END}, .size = 1},
	{.opcode = 0x95, .name = "xchg bp,ax", .mode = {END}, .size = 1},
	{.opcode = 0x96, .name = "xchg si,ax", .mode = {END}, .size = 1},
	{.opcode = 0x97, .name = "xchg di,ax", .mode = {END}, .size = 1},
	{.opcode = 0x98, .name = "cbw", .mode = {END}, .size = 1},
	{.opcode = 0x99, .name = "cwd", .mode = {END}, .size = 1},
	// {.opcode = 0x9A, .name = "call", .mode = {CALL_INTER, END}, .size = 1},
	{.opcode = 0x9B, .name = "wait", .mode = {END}, .size = 1},
	{.opcode = 0x9C, .name = "pushf", .mode = {END}, .size = 1},
	{.opcode = 0x9D, .name = "popf", .mode = {END}, .size = 1},
	{.opcode = 0x9E, .name = "sahf", .mode = {END}, .size = 1},
	{.opcode = 0x9F, .name = "lahf", .mode = {END}, .size = 1},
	// {.opcode = 0xA0, .name = "mov al,%s", .mode = {moffs16, END}, .size = 1},
	// {.opcode = 0xA1, .name = "mov ax,%s", .mode = {moffs16, END}, .size = 1},
	// {.opcode = 0xA2, .name = "mov %s,al", .mode = {moffs16, END}, .size = 1},
	// {.opcode = 0xA3, .name = "mov %s,ax", .mode = {moffs16, END}, .size = 1},
	{.opcode = 0xA4, .name = "movsb", .mode = {END}, .size = 1},
	{.opcode = 0xA5, .name = "movsw", .mode = {END}, .size = 1},
	{.opcode = 0xA6, .name = "cmpsb", .mode = {END}, .size = 1},
	{.opcode = 0xA7, .name = "cmpsw", .mode = {END}, .size = 1},
	{.opcode = 0xA8, .name = "test al,", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xA9, .name = "test ax,", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0xAA, .name = "stosb", .mode = {END}, .size = 1},
	{.opcode = 0xAB, .name = "stosw", .mode = {END}, .size = 1},
	{.opcode = 0xAC, .name = "lodsb", .mode = {END}, .size = 1},
	{.opcode = 0xAD, .name = "lodsw", .mode = {END}, .size = 1},
	{.opcode = 0xAE, .name = "scasb", .mode = {END}, .size = 1},
	{.opcode = 0xAF, .name = "scasw", .mode = {END}, .size = 1},

	{.opcode = 0xB0, .name = "mov al", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xB1, .name = "mov cl", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xB2, .name = "mov dl", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xB3, .name = "mov bl", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xB4, .name = "mov ah", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xB5, .name = "mov ch", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xB6, .name = "mov dh", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xB7, .name = "mov bh", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xB8, .name = "mov ax", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0xB9, .name = "mov cx", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0xBA, .name = "mov dx", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0xBB, .name = "mov bx", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0xBC, .name = "mov sp", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0xBD, .name = "mov bp", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0xBE, .name = "mov si", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0xBF, .name = "mov di", .mode = {IMM16, END}, .size = 3},

	{.opcode = 0xC2, .name = "ret", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0xC3, .name = "ret", .mode = {END}, .size = 1},
	{.opcode = 0xC4, .name = "les", .mode = {REG16, R_M16, END}, .size = 3},
	{.opcode = 0xC5, .name = "lds", .mode = {R_M16, REG16, END}, .size = 3},
	{.opcode = 0xC6, .name = "mov", .mode = {R_M16, IMM8, END}, .size = 4},
	{.opcode = 0xC7, .name = "mov", .mode = {R_M16, IMM16, END}, .size = 5},
	{.opcode = 0xCA, .name = "retf", .mode = {IMM16, END}, .size = 3},
	{.opcode = 0xCB, .name = "retf", .mode = {END}, .size = 1},
	{.opcode = 0xCC, .name = "int3", .mode = {END}, .size = 1},
	{.opcode = 0xCD, .name = "int", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xCE, .name = "into", .mode = {END}, .size = 1},
	{.opcode = 0xCF, .name = "iret", .mode = {END}, .size = 1},

	{.opcode = 0xD4, .name = "aam", .mode = {END}, .size = 1},
	{.opcode = 0xD5, .name = "aad", .mode = {END}, .size = 1},
	{.opcode = 0xD7, .name = "xlatb", .mode = {END}, .size = 1},

	{.opcode = 0xE0, .name = "loopne", .mode = {REL8, END}, .size = 2},
	{.opcode = 0xE1, .name = "loope", .mode = {REL8, END}, .size = 2},
	{.opcode = 0xE2, .name = "loop", .mode = {REL8, END}, .size = 2},
	{.opcode = 0xE3, .name = "jcxz", .mode = {REL8, END}, .size = 2},
	{.opcode = 0xE4, .name = "in al,%s", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xE5, .name = "in ax,%s", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xE6, .name = "out %s,al", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xE7, .name = "out %s,ax", .mode = {IMM8, END}, .size = 2},
	{.opcode = 0xE8, .name = "call", .mode = {REL16, END}, .size = 3},
	{.opcode = 0xE9, .name = "jmp", .mode = {REL16, END}, .size = 3},
	// {.opcode = 0xEA, .name = "jmp", .mode = {CALL_INTER, END}, .size = 1},
	{.opcode = 0xEB, .name = "jmp short", .mode = {REL8, END}, .size = 2},
	{.opcode = 0xEC, .name = "in al,dx", .mode = {END}, .size = 1},
	{.opcode = 0xED, .name = "in ax,dx", .mode = {END}, .size = 1},
	{.opcode = 0xEE, .name = "out dx,al", .mode = {END}, .size = 1},
	{.opcode = 0xEF, .name = "out dx,ax", .mode = {END}, .size = 1},
	{.opcode = 0xF0, .name = "lock", .mode = {END}, .size = 1},
	{.opcode = 0xF2, .name = "repne", .mode = {END}, .size = 1},
	{.opcode = 0xF3, .name = "rep", .mode = {END}, .size = 1},
	{.opcode = 0xF4, .name = "hlt", .mode = {END}, .size = 1},
	{.opcode = 0xF5, .name = "cmc", .mode = {END}, .size = 1},

	{.opcode = 0xF8, .name = "clc", .mode = {END}, .size = 1},
	{.opcode = 0xF9, .name = "stc", .mode = {END}, .size = 1},
	{.opcode = 0xFA, .name = "cli", .mode = {END}, .size = 1},
	{.opcode = 0xFB, .name = "sti", .mode = {END}, .size = 1},
	{.opcode = 0xFC, .name = "cld", .mode = {END}, .size = 1},
	{.opcode = 0xFD, .name = "std", .mode = {END}, .size = 1},

	{.name = NULL}
};
