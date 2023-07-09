#include <stddef.h>
#include <stdio.h>
#include "dasm.h"
#include "instructions.h"

instruction_t parse_inst(u_int8_t *binary, unsigned long size)
{
	for (int i = 0; instructions[i].name; i++) {
		if (instructions[i].opcode != *binary)
			continue;

		if (instructions[i].extended == -1)
			return instructions[i];

		if (size < 2) {
			dprintf(2, "Invalid extended instruction.\n");
			return invalid_instruction;
		}
		unsigned mod = (binary[1] & 0b111000) >> 3;
		return extended[instructions[i].extended][mod];
	}
	dprintf(2, "Not implemented instruction: %x\n", *binary);
	return invalid_instruction;
}

const instruction_t instructions[] = {
	{.opcode = 0x00, .extended = -1, .name = "add", .mode = {R_M8, REG8, END}, .exec = NULL},
	{.opcode = 0x01, .extended = -1, .name = "add", .mode = {R_M16, REG16, END}, .exec = NULL},
	{.opcode = 0x02, .extended = -1, .name = "add", .mode = {REG8, R_M8, END}, .exec = NULL},
	{.opcode = 0x03, .extended = -1, .name = "add", .mode = {REG16, R_M16, END}, .exec = NULL},
	{.opcode = 0x04, .extended = -1, .name = "add", .mode = {OPREG8, IMM8, END}, .exec = NULL},
	{.opcode = 0x05, .extended = -1, .name = "add", .mode = {OPREG16, IMM16, END}, .exec = NULL},
	{.opcode = 0x06, .extended = -1, .name = "push es", .mode = {END}, .exec = &push},
	{.opcode = 0x07, .extended = -1, .name = "pop es", .mode = {END}, .exec = NULL},
	{.opcode = 0x08, .extended = -1, .name = "or", .mode = {R_M8, REG8, END}, .exec = NULL},
	{.opcode = 0x09, .extended = -1, .name = "or", .mode = {R_M16, REG16, END}, .exec = NULL},
	{.opcode = 0x0A, .extended = -1, .name = "or", .mode = {REG8, R_M8, END}, .exec = NULL},
	{.opcode = 0x0B, .extended = -1, .name = "or", .mode = {REG16, R_M16, END}, .exec = NULL},
	{.opcode = 0x0C, .extended = -1, .name = "or", .mode = {OPREG8, IMM8, END}, .exec = NULL},
	{.opcode = 0x0D, .extended = -1, .name = "or", .mode = {OPREG16, IMM16, END}, .exec = NULL},
	{.opcode = 0x0E, .extended = -1, .name = "push cs", .mode = {END}, .exec = &push},

	{.opcode = 0x10, .extended = -1, .name = "adc", .mode = {R_M8, REG8, END}, .exec = NULL},
	{.opcode = 0x11, .extended = -1, .name = "adc", .mode = {R_M16, REG16, END}, .exec = NULL},
	{.opcode = 0x12, .extended = -1, .name = "adc", .mode = {REG8, R_M8, END}, .exec = NULL},
	{.opcode = 0x13, .extended = -1, .name = "adc", .mode = {REG16, R_M16, END}, .exec = NULL},
	{.opcode = 0x14, .extended = -1, .name = "adc", .mode = {OPREG8, IMM8, END}, .exec = NULL},
	{.opcode = 0x15, .extended = -1, .name = "adc", .mode = {OPREG16, IMM16, END}, .exec = NULL},
	{.opcode = 0x16, .extended = -1, .name = "push ss", .mode = {END}, .exec = &push},
	{.opcode = 0x17, .extended = -1, .name = "pop ss", .mode = {END}, .exec = NULL},
	{.opcode = 0x18, .extended = -1, .name = "sbb", .mode = {R_M8, REG8, END}, .exec = NULL},
	{.opcode = 0x19, .extended = -1, .name = "sbb", .mode = {R_M16, REG16, END}, .exec = NULL},
	{.opcode = 0x1A, .extended = -1, .name = "sbb", .mode = {REG8, R_M8, END}, .exec = NULL},
	{.opcode = 0x1B, .extended = -1, .name = "sbb", .mode = {REG16, R_M16, END}, .exec = NULL},
	{.opcode = 0x1C, .extended = -1, .name = "sbb", .mode = {OPREG8, IMM8, END}, .exec = NULL},
	{.opcode = 0x1D, .extended = -1, .name = "sbb", .mode = {OPREG16, IMM16, END}, .exec = NULL},
	{.opcode = 0x1E, .extended = -1, .name = "push ds", .mode = {END}, .exec = &push},
	{.opcode = 0x1F, .extended = -1, .name = "pop ds", .mode = {END}, .exec = NULL},
	{.opcode = 0x20, .extended = -1, .name = "and", .mode = {R_M8, REG8, END}, .exec = NULL},
	{.opcode = 0x21, .extended = -1, .name = "and", .mode = {R_M16, REG16, END}, .exec = NULL},
	{.opcode = 0x22, .extended = -1, .name = "and", .mode = {REG8, R_M8, END}, .exec = NULL},
	{.opcode = 0x23, .extended = -1, .name = "and", .mode = {REG16, R_M16, END}, .exec = NULL},
	{.opcode = 0x24, .extended = -1, .name = "and", .mode = {OPREG8, IMM8, END}, .exec = NULL},
	{.opcode = 0x25, .extended = -1, .name = "and", .mode = {OPREG16, IMM16, END}, .exec = NULL},

	{.opcode = 0x27, .extended = -1, .name = "daa", .mode = {END}, .exec = NULL},
	{.opcode = 0x28, .extended = -1, .name = "sub", .mode = {R_M8, REG8, END}, .exec = &sub},
	{.opcode = 0x29, .extended = -1, .name = "sub", .mode = {R_M16, REG16, END}, .exec = &sub},
	{.opcode = 0x2A, .extended = -1, .name = "sub", .mode = {REG8, R_M8, END}, .exec = &sub},
	{.opcode = 0x2B, .extended = -1, .name = "sub", .mode = {REG16, R_M16, END}, .exec = &sub},
	{.opcode = 0x2C, .extended = -1, .name = "sub", .mode = {OPREG8, IMM8, END}, .exec = &sub},
	{.opcode = 0x2D, .extended = -1, .name = "sub", .mode = {OPREG16, IMM16, END}, .exec = &sub},

	{.opcode = 0x2F, .extended = -1, .name = "das", .mode = {END}, .exec = NULL},
	{.opcode = 0x30, .extended = -1, .name = "xor", .mode = {R_M8, REG8, END}, .exec = &xor_inst},
	{.opcode = 0x31, .extended = -1, .name = "xor", .mode = {R_M16, REG16, END}, .exec = &xor_inst},
	{.opcode = 0x32, .extended = -1, .name = "xor", .mode = {REG8, R_M8, END}, .exec = &xor_inst},
	{.opcode = 0x33, .extended = -1, .name = "xor", .mode = {REG16, R_M16, END}, .exec = &xor_inst},
	{.opcode = 0x34, .extended = -1, .name = "xor", .mode = {OPREG8, IMM8, END}, .exec = &xor_inst},
	{.opcode = 0x35, .extended = -1, .name = "xor", .mode = {OPREG16, IMM16, END}, .exec = &xor_inst},

	{.opcode = 0x37, .extended = -1, .name = "aaa", .mode = {END}, .exec = NULL},
	{.opcode = 0x38, .extended = -1, .name = "cmp", .mode = {R_M8, REG8, END}, .exec = NULL},
	{.opcode = 0x39, .extended = -1, .name = "cmp", .mode = {R_M16, REG16, END}, .exec = NULL},
	{.opcode = 0x3A, .extended = -1, .name = "cmp", .mode = {REG8, R_M8, END}, .exec = NULL},
	{.opcode = 0x3B, .extended = -1, .name = "cmp", .mode = {REG16, R_M16, END}, .exec = NULL},
	{.opcode = 0x3C, .extended = -1, .name = "cmp", .mode = {OPREG8, IMM8, END}, .exec = NULL},
	{.opcode = 0x3D, .extended = -1, .name = "cmp", .mode = {OPREG16, IMM16, END}, .exec = NULL},

	{.opcode = 0x3F, .extended = -1, .name = "ass", .mode = {END}, .exec = NULL},
	{.opcode = 0x40, .extended = -1, .name = "inc", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x41, .extended = -1, .name = "inc", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x42, .extended = -1, .name = "inc", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x43, .extended = -1, .name = "inc", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x44, .extended = -1, .name = "inc", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x45, .extended = -1, .name = "inc", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x46, .extended = -1, .name = "inc", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x47, .extended = -1, .name = "inc", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x48, .extended = -1, .name = "dec", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x49, .extended = -1, .name = "dec", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x4A, .extended = -1, .name = "dec", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x4B, .extended = -1, .name = "dec", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x4C, .extended = -1, .name = "dec", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x4D, .extended = -1, .name = "dec", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x4E, .extended = -1, .name = "dec", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x4F, .extended = -1, .name = "dec", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x50, .extended = -1, .name = "push", .mode = {OPREG16, END}, .exec = &push},
	{.opcode = 0x51, .extended = -1, .name = "push", .mode = {OPREG16, END}, .exec = &push},
	{.opcode = 0x52, .extended = -1, .name = "push", .mode = {OPREG16, END}, .exec = &push},
	{.opcode = 0x53, .extended = -1, .name = "push", .mode = {OPREG16, END}, .exec = &push},
	{.opcode = 0x54, .extended = -1, .name = "push", .mode = {OPREG16, END}, .exec = &push},
	{.opcode = 0x55, .extended = -1, .name = "push", .mode = {OPREG16, END}, .exec = &push},
	{.opcode = 0x56, .extended = -1, .name = "push", .mode = {OPREG16, END}, .exec = &push},
	{.opcode = 0x57, .extended = -1, .name = "push", .mode = {OPREG16, END}, .exec = &push},
	{.opcode = 0x58, .extended = -1, .name = "pop", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x59, .extended = -1, .name = "pop", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x5A, .extended = -1, .name = "pop", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x5B, .extended = -1, .name = "pop", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x5C, .extended = -1, .name = "pop", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x5D, .extended = -1, .name = "pop", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x5E, .extended = -1, .name = "pop", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x5F, .extended = -1, .name = "pop", .mode = {OPREG16, END}, .exec = NULL},
	{.opcode = 0x70, .extended = -1, .name = "jo", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x71, .extended = -1, .name = "jno", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x72, .extended = -1, .name = "jb", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x73, .extended = -1, .name = "jnb", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x74, .extended = -1, .name = "je", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x75, .extended = -1, .name = "jne", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x76, .extended = -1, .name = "jbe", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x77, .extended = -1, .name = "jnbe", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x78, .extended = -1, .name = "js", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x79, .extended = -1, .name = "jns", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x7A, .extended = -1, .name = "jpe", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x7B, .extended = -1, .name = "jpo", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x7C, .extended = -1, .name = "jl", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x7D, .extended = -1, .name = "jnl", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x7E, .extended = -1, .name = "jng", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x7F, .extended = -1, .name = "jg", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0x80, .extended = 0,  .name = "EXTENDED", .mode = {END}, .exec = NULL},
	{.opcode = 0x81, .extended = 1,  .name = "EXTENDED", .mode = {END}, .exec = NULL},

	{.opcode = 0x83, .extended = 2,  .name = "EXTENDED", .mode = {END}, .exec = NULL},
	{.opcode = 0x84, .extended = -1, .name = "test", .mode = {R_M8, REG8, END}, .exec = NULL},
	{.opcode = 0x85, .extended = -1, .name = "test", .mode = {R_M16, REG16, END}, .exec = NULL},
	{.opcode = 0x86, .extended = -1, .name = "xchg", .mode = {R_M8, REG8, END}, .exec = NULL},
	{.opcode = 0x87, .extended = -1, .name = "xchg", .mode = {R_M16, REG16, END}, .exec = NULL},
	{.opcode = 0x88, .extended = -1, .name = "mov", .mode = {R_M8, REG8, END}, .exec = &mov},
	{.opcode = 0x89, .extended = -1, .name = "mov", .mode = {R_M16, REG16, END}, .exec = &mov},
	{.opcode = 0x8A, .extended = -1, .name = "mov", .mode = {REG8, R_M8, END}, .exec = &mov},
	{.opcode = 0x8B, .extended = -1, .name = "mov", .mode = {REG16, R_M16, END}, .exec = &mov},
	// {.opcode = 0x8C, .extended = -1, .name = "mov", .mode = {R_M16, sreg, END}, .exec = &mov},
	{.opcode = 0x8D, .extended = -1, .name = "lea", .mode = {REG16, R_M16, END}, .exec = &lea},
	// {.opcode = 0x8E, .extended = -1, .name = "mov", .mode = {sreg, R_M16, END}, .exec = &mov},
	{.opcode = 0x8F, .extended = -1, .name = "pop", .mode = {R_M16, END}, .exec = NULL},
	{.opcode = 0x90, .extended = -1, .name = "xchg ax, ax", .mode = {END}, .exec = NULL},
	{.opcode = 0x91, .extended = -1, .name = "xchg cx, ax", .mode = {END}, .exec = NULL},
	{.opcode = 0x92, .extended = -1, .name = "xchg dx, ax", .mode = {END}, .exec = NULL},
	{.opcode = 0x93, .extended = -1, .name = "xchg bx, ax", .mode = {END}, .exec = NULL},
	{.opcode = 0x94, .extended = -1, .name = "xchg sp, ax", .mode = {END}, .exec = NULL},
	{.opcode = 0x95, .extended = -1, .name = "xchg bp, ax", .mode = {END}, .exec = NULL},
	{.opcode = 0x96, .extended = -1, .name = "xchg si, ax", .mode = {END}, .exec = NULL},
	{.opcode = 0x97, .extended = -1, .name = "xchg di, ax", .mode = {END}, .exec = NULL},
	{.opcode = 0x98, .extended = -1, .name = "cbw", .mode = {END}, .exec = NULL},
	{.opcode = 0x99, .extended = -1, .name = "cwd", .mode = {END}, .exec = NULL},
	// {.opcode = 0x9A, .extended = -1, .name = "call", .mode = {CALL_INTER, END}, .exec = &call},
	{.opcode = 0x9B, .extended = -1, .name = "wait", .mode = {END}, .exec = NULL},
	{.opcode = 0x9C, .extended = -1, .name = "pushf", .mode = {END}, .exec = &push},
	{.opcode = 0x9D, .extended = -1, .name = "popf", .mode = {END}, .exec = NULL},
	{.opcode = 0x9E, .extended = -1, .name = "sahf", .mode = {END}, .exec = NULL},
	{.opcode = 0x9F, .extended = -1, .name = "lahf", .mode = {END}, .exec = NULL},
	// {.opcode = 0xA0, .extended = -1, .name = "mov al", .mode = {moffs16, END}, .exec = &mov},
	// {.opcode = 0xA1, .extended = -1, .name = "mov ax", .mode = {moffs16, END}, .exec = &mov},
	// {.opcode = 0xA2, .extended = -1, .name = "mov %s, al", .mode = {moffs16, END}, .exec = &mov},
	// {.opcode = 0xA3, .extended = -1, .name = "mov %s, ax", .mode = {moffs16, END}, .exec = &mov},
	{.opcode = 0xA4, .extended = -1, .name = "movsb", .mode = {END}, .exec = NULL},
	{.opcode = 0xA5, .extended = -1, .name = "movsw", .mode = {END}, .exec = NULL},
	{.opcode = 0xA6, .extended = -1, .name = "cmpsb", .mode = {END}, .exec = NULL},
	{.opcode = 0xA7, .extended = -1, .name = "cmpsw", .mode = {END}, .exec = NULL},
	{.opcode = 0xA8, .extended = -1, .name = "test al,", .mode = {IMM8, END}, .exec = NULL},
	{.opcode = 0xA9, .extended = -1, .name = "test ax,", .mode = {IMM16, END}, .exec = NULL},
	{.opcode = 0xAA, .extended = -1, .name = "stosb", .mode = {END}, .exec = NULL},
	{.opcode = 0xAB, .extended = -1, .name = "stosw", .mode = {END}, .exec = NULL},
	{.opcode = 0xAC, .extended = -1, .name = "lodsb", .mode = {END}, .exec = NULL},
	{.opcode = 0xAD, .extended = -1, .name = "lodsw", .mode = {END}, .exec = NULL},
	{.opcode = 0xAE, .extended = -1, .name = "scasb", .mode = {END}, .exec = NULL},
	{.opcode = 0xAF, .extended = -1, .name = "scasw", .mode = {END}, .exec = NULL},
	{.opcode = 0xB0, .extended = -1, .name = "mov", .mode = {OPREG8, IMM8, END}, .exec = &mov},
	{.opcode = 0xB1, .extended = -1, .name = "mov", .mode = {OPREG8, IMM8, END}, .exec = &mov},
	{.opcode = 0xB2, .extended = -1, .name = "mov", .mode = {OPREG8, IMM8, END}, .exec = &mov},
	{.opcode = 0xB3, .extended = -1, .name = "mov", .mode = {OPREG8, IMM8, END}, .exec = &mov},
	{.opcode = 0xB4, .extended = -1, .name = "mov", .mode = {OPREG8, IMM8, END}, .exec = &mov},
	{.opcode = 0xB5, .extended = -1, .name = "mov", .mode = {OPREG8, IMM8, END}, .exec = &mov},
	{.opcode = 0xB6, .extended = -1, .name = "mov", .mode = {OPREG8, IMM8, END}, .exec = &mov},
	{.opcode = 0xB7, .extended = -1, .name = "mov", .mode = {OPREG8, IMM8, END}, .exec = &mov},
	{.opcode = 0xB8, .extended = -1, .name = "mov", .mode = {OPREG16, IMM16, END}, .exec = &mov},
	{.opcode = 0xB9, .extended = -1, .name = "mov", .mode = {OPREG16, IMM16, END}, .exec = &mov},
	{.opcode = 0xBA, .extended = -1, .name = "mov", .mode = {OPREG16, IMM16, END}, .exec = &mov},
	{.opcode = 0xBB, .extended = -1, .name = "mov", .mode = {OPREG16, IMM16, END}, .exec = &mov},
	{.opcode = 0xBC, .extended = -1, .name = "mov", .mode = {OPREG16, IMM16, END}, .exec = &mov},
	{.opcode = 0xBD, .extended = -1, .name = "mov", .mode = {OPREG16, IMM16, END}, .exec = &mov},
	{.opcode = 0xBE, .extended = -1, .name = "mov", .mode = {OPREG16, IMM16, END}, .exec = &mov},
	{.opcode = 0xBF, .extended = -1, .name = "mov", .mode = {OPREG16, IMM16, END}, .exec = &mov},

	{.opcode = 0xC2, .extended = -1, .name = "ret", .mode = {IMM16, END}, .exec = NULL},
	{.opcode = 0xC3, .extended = -1, .name = "ret", .mode = {END}, .exec = NULL},
	{.opcode = 0xC4, .extended = -1, .name = "les", .mode = {REG16, R_M16, END}, .exec = NULL},
	{.opcode = 0xC5, .extended = -1, .name = "lds", .mode = {R_M16, REG16, END}, .exec = NULL},
	{.opcode = 0xC6, .extended = -1, .name = "mov", .mode = {R_M16, IMM8, END}, .exec = &mov},
	{.opcode = 0xC7, .extended = -1, .name = "mov", .mode = {R_M16, IMM16, END}, .exec = &mov},
	{.opcode = 0xCA, .extended = -1, .name = "retf", .mode = {IMM16, END}, .exec = NULL},
	{.opcode = 0xCB, .extended = -1, .name = "retf", .mode = {END}, .exec = NULL},
	{.opcode = 0xCC, .extended = -1, .name = "int3", .mode = {END}, .exec = NULL},
	{.opcode = 0xCD, .extended = -1, .name = "int", .mode = {IMM8, END}, .exec = &int_inst},
	{.opcode = 0xCE, .extended = -1, .name = "into", .mode = {END}, .exec = NULL},
	{.opcode = 0xCF, .extended = -1, .name = "iret", .mode = {END}, .exec = NULL},
	{.opcode = 0xD0, .extended =  7, .name = "EXTENDED", .mode = {END}, .exec = NULL},
	{.opcode = 0xD1, .extended =  8, .name = "EXTENDED", .mode = {END}, .exec = NULL},
	{.opcode = 0xD2, .extended =  9, .name = "EXTENDED", .mode = {END}, .exec = NULL},
	{.opcode = 0xD3, .extended = 10, .name = "EXTENDED", .mode = {END}, .exec = NULL},
	{.opcode = 0xD4, .extended = -1, .name = "aam", .mode = {END}, .exec = NULL},
	{.opcode = 0xD5, .extended = -1, .name = "aad", .mode = {END}, .exec = NULL},
	{.opcode = 0xD7, .extended = -1, .name = "xlatb", .mode = {END}, .exec = NULL},

	{.opcode = 0xE0, .extended = -1, .name = "loopne", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0xE1, .extended = -1, .name = "loope", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0xE2, .extended = -1, .name = "loop", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0xE3, .extended = -1, .name = "jcxz", .mode = {REL8, END}, .exec = NULL},
	{.opcode = 0xE4, .extended = -1, .name = "in al", .mode = {IMM8, END}, .exec = NULL},
	{.opcode = 0xE5, .extended = -1, .name = "in ax", .mode = {IMM8, END}, .exec = NULL},
	{.opcode = 0xE6, .extended = -1, .name = "out %s, al", .mode = {IMM8, END}, .exec = NULL},
	{.opcode = 0xE7, .extended = -1, .name = "out %s, ax", .mode = {IMM8, END}, .exec = NULL},
	{.opcode = 0xE8, .extended = -1, .name = "call", .mode = {REL16, END}, .exec = &call},
	{.opcode = 0xE9, .extended = -1, .name = "jmp", .mode = {REL16, END}, .exec = &jmp},
	// {.opcode = 0xEA, .extended = -1, .name = "jmp", .mode = {CALL_INTER, END}, .exec = &jmp},
	{.opcode = 0xEB, .extended = -1, .name = "jmp short", .mode = {REL8, END}, .exec = &jmp},
	{.opcode = 0xEC, .extended = -1, .name = "in al, dx", .mode = {END}, .exec = NULL},
	{.opcode = 0xED, .extended = -1, .name = "in ax, dx", .mode = {END}, .exec = NULL},
	{.opcode = 0xEE, .extended = -1, .name = "out dx, al", .mode = {END}, .exec = NULL},
	{.opcode = 0xEF, .extended = -1, .name = "out dx, ax", .mode = {END}, .exec = NULL},
	{.opcode = 0xF0, .extended = -1, .name = "lock", .mode = {END}, .exec = NULL},
	{.opcode = 0xF2, .extended = -1, .name = "repne", .mode = {END}, .exec = NULL},
	{.opcode = 0xF3, .extended = -1, .name = "rep", .mode = {END}, .exec = NULL},
	{.opcode = 0xF4, .extended = -1, .name = "hlt", .mode = {END}, .exec = NULL},
	{.opcode = 0xF5, .extended = -1, .name = "cmc", .mode = {END}, .exec = NULL},
	{.opcode = 0xF6, .extended = 3,  .name = "EXTENDED", .mode = {END}, .exec = NULL},
	{.opcode = 0xF7, .extended = 4,  .name = "EXTENDED", .mode = {END}, .exec = NULL},
	{.opcode = 0xF8, .extended = -1, .name = "clc", .mode = {END}, .exec = NULL},
	{.opcode = 0xF9, .extended = -1, .name = "stc", .mode = {END}, .exec = NULL},
	{.opcode = 0xFA, .extended = -1, .name = "cli", .mode = {END}, .exec = NULL},
	{.opcode = 0xFB, .extended = -1, .name = "sti", .mode = {END}, .exec = NULL},
	{.opcode = 0xFC, .extended = -1, .name = "cld", .mode = {END}, .exec = NULL},
	{.opcode = 0xFD, .extended = -1, .name = "std", .mode = {END}, .exec = NULL},
	{.opcode = 0xFE, .extended = 5,  .name = "EXTENDED", .mode = {END}, .exec = NULL},
	{.opcode = 0xFF, .extended = 6,  .name = "EXTENDED", .mode = {END}, .exec = NULL},
	{.name = NULL}
};

const instruction_t extended[][8] = {
	// 0x80 extended
	{
		{.opcode = 0x00, .extended = -2, .name = "add", .mode = {R_M8, IMM8, END}, .exec = NULL},
		{.opcode = 0x01, .extended = -2, .name = "or", .mode = {R_M8, IMM8, END}, .exec = NULL},
		{.opcode = 0x02, .extended = -2, .name = "adc", .mode = {R_M8, IMM8, END}, .exec = NULL},
		{.opcode = 0x03, .extended = -2, .name = "sbb", .mode = {R_M8, IMM8, END}, .exec = NULL},
		{.opcode = 0x04, .extended = -2, .name = "and", .mode = {R_M8, IMM8, END}, .exec = NULL},
		{.opcode = 0x05, .extended = -2, .name = "sub", .mode = {R_M8, IMM8, END}, .exec = &sub},
		{.opcode = 0x06, .extended = -2, .name = "xor", .mode = {R_M8, IMM8, END}, .exec = &xor_inst},
		{.opcode = 0x07, .extended = -2, .name = "cmp", .mode = {R_M8, IMM8, END}, .exec = NULL},
	},
	// 0x81 extended
	{
		{.opcode = 0x00, .extended = -2, .name = "add", .mode = {R_M16, IMM16, END}, .exec = NULL},
		{.opcode = 0x01, .extended = -2, .name = "or", .mode = {R_M16, IMM16, END}, .exec = NULL},
		{.opcode = 0x02, .extended = -2, .name = "adc", .mode = {R_M16, IMM16, END}, .exec = NULL},
		{.opcode = 0x03, .extended = -2, .name = "sbb", .mode = {R_M16, IMM16, END}, .exec = NULL},
		{.opcode = 0x04, .extended = -2, .name = "and", .mode = {R_M16, IMM16, END}, .exec = NULL},
		{.opcode = 0x05, .extended = -2, .name = "sub", .mode = {R_M16, IMM16, END}, .exec = &sub},
		{.opcode = 0x06, .extended = -2, .name = "xor", .mode = {R_M16, IMM16, END}, .exec = &xor_inst},
		{.opcode = 0x07, .extended = -2, .name = "cmp", .mode = {R_M16, IMM16, END}, .exec = NULL},
	},
	// 0x83 extended
	{
		{.opcode = 0x00, .extended = -2, .name = "add", .mode = {R_M16, IMM8, END}, .exec = NULL},
		/**/{.opcode = 0x01, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		{.opcode = 0x02, .extended = -2, .name = "adc", .mode = {R_M16, IMM8, END}, .exec = NULL},
		{.opcode = 0x03, .extended = -2, .name = "sbb", .mode = {R_M16, IMM8, END}, .exec = NULL},
		/**/{.opcode = 0x04, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		{.opcode = 0x05, .extended = -2, .name = "sub", .mode = {R_M16, IMM8, END}, .exec = &sub},
		/**/{.opcode = 0x06, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		{.opcode = 0x07, .extended = -2, .name = "cmp", .mode = {R_M16, IMM8, END}, .exec = NULL},
	},
	// 0xf6 extended
	{
		{.opcode = 0x00, .extended = -2, .name = "test", .mode = {R_M8, IMM8, END}, .exec = NULL},
		/**/{.opcode = 0x01, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		{.opcode = 0x02, .extended = -2, .name = "not", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x03, .extended = -2, .name = "neg", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x04, .extended = -2, .name = "mul", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x05, .extended = -2, .name = "imul", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x06, .extended = -2, .name = "div", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x07, .extended = -2, .name = "idiv", .mode = {R_M8, END}, .exec = NULL},
	},
	// 0xf7 extended
	{
		{.opcode = 0x00, .extended = -2, .name = "test", .mode = {R_M16, IMM16, END}, .exec = NULL},
		/**/{.opcode = 0x01, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		{.opcode = 0x02, .extended = -2, .name = "not", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x03, .extended = -2, .name = "neg", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x04, .extended = -2, .name = "mul", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x05, .extended = -2, .name = "imul", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x06, .extended = -2, .name = "div", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x07, .extended = -2, .name = "idiv", .mode = {R_M16, END}, .exec = NULL},
	},
	// 0xfe extended
	{
		{.opcode = 0x00, .extended = -2, .name = "inc", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x01, .extended = -2, .name = "dec", .mode = {R_M8, END}, .exec = NULL},
		/**/{.opcode = 0x02, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		/**/{.opcode = 0x03, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		/**/{.opcode = 0x04, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		/**/{.opcode = 0x05, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		/**/{.opcode = 0x06, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		/**/{.opcode = 0x07, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
	},
	// 0xff extended
	{
		{.opcode = 0x00, .extended = -2, .name = "inc", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x01, .extended = -2, .name = "dec", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x02, .extended = -2, .name = "call", .mode = {R_M16, END}, .exec = &call},
		{.opcode = 0x03, .extended = -2, .name = "call", .mode = {R_M16, END}, .exec = &call},
		{.opcode = 0x04, .extended = -2, .name = "jmp", .mode = {R_M16, END}, .exec = &jmp},
		{.opcode = 0x05, .extended = -2, .name = "jmp", .mode = {R_M16, END}, .exec = &jmp},
		{.opcode = 0x06, .extended = -2, .name = "push", .mode = {R_M16, END}, .exec = &push},
		/**/{.opcode = 0x07, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
	},

	// 0xd0 extended
	{
		{.opcode = 0x00, .extended = -2, .name = "rol %s, 1", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x01, .extended = -2, .name = "ror %s, 1", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x02, .extended = -2, .name = "rcl %s, 1", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x03, .extended = -2, .name = "rcr %s, 1", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x04, .extended = -2, .name = "shl %s, 1", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x05, .extended = -2, .name = "shr %s, 1", .mode = {R_M8, END}, .exec = NULL},
		/**/{.opcode = 0x06, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		{.opcode = 0x07, .extended = -2, .name = "sar %s, 1", .mode = {R_M8, END}, .exec = NULL},
	},
	// 0xd1 extended
	{
		{.opcode = 0x00, .extended = -2, .name = "rol %s, 1", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x01, .extended = -2, .name = "ror %s, 1", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x02, .extended = -2, .name = "rcl %s, 1", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x03, .extended = -2, .name = "rcr %s, 1", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x04, .extended = -2, .name = "shl %s, 1", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x05, .extended = -2, .name = "shr %s, 1", .mode = {R_M16, END}, .exec = NULL},
		/**/{.opcode = 0x06, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		{.opcode = 0x07, .extended = -2, .name = "sar %s, 1", .mode = {R_M16, END}, .exec = NULL},
	},
	// 0xd2 extended
	{
		{.opcode = 0x00, .extended = -2, .name = "rol %s, cl", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x01, .extended = -2, .name = "ror %s, cl", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x02, .extended = -2, .name = "rcl %s, cl", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x03, .extended = -2, .name = "rcr %s, cl", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x04, .extended = -2, .name = "shl %s, cl", .mode = {R_M8, END}, .exec = NULL},
		{.opcode = 0x05, .extended = -2, .name = "shr %s, cl", .mode = {R_M8, END}, .exec = NULL},
		/**/{.opcode = 0x06, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		{.opcode = 0x07, .extended = -2, .name = "sar %s, cl", .mode = {R_M8, END}, .exec = NULL},
	},
	// 0xd3 extended
	{
		{.opcode = 0x00, .extended = -2, .name = "rol %s, cl", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x01, .extended = -2, .name = "ror %s, cl", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x02, .extended = -2, .name = "rcl %s, cl", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x03, .extended = -2, .name = "rcr %s, cl", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x04, .extended = -2, .name = "shl %s, cl", .mode = {R_M16, END}, .exec = NULL},
		{.opcode = 0x05, .extended = -2, .name = "shr %s, cl", .mode = {R_M16, END}, .exec = NULL},
		/**/{.opcode = 0x06, .extended = -2, .name = "invalid", .mode = {END}, .exec = NULL},
		{.opcode = 0x07, .extended = -2, .name = "sar %s, cl", .mode = {R_M16, END}, .exec = NULL},
	},
};

const instruction_t invalid_instruction = { .opcode = 0xFF, .name = "unknown", .mode = {END}};

