#include <stddef.h>
#include "dasm.h"

const instruction_t instructions[] = {
	{.opcode = 0x00, .name = "add", .mode = {R_M8, REG8, END}, .size = 2},
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
	{.name = NULL}
};
