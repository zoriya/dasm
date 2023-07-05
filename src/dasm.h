#pragma once

#include "interpretor.h"
#include <sys/types.h>
#include <stdbool.h>

int dasm(u_int8_t *binary, unsigned long size);
int interpret(u_int8_t *binary, unsigned long size);

// typedef enum addressing_mode {
// 	/// Param is the data (MOV AL, ~68FE~)
// 	IMMEDIATE,
// 	REGISTER,
// 	DIRECT,
// 	REGISTER_INDIRECT,
// 	BASED,
// 	INDEXED,
// 	BASED_INDEX,
// 	BASED_INDEX_DISPLACEMENT,
// } addressing_mode_t;

typedef enum addressing_mode {
	// 8bit register
	REG8,
	// 16bit register
	REG16,
	// Register or 8bit of memory
	R_M8,
	// Register or 16bit of memory
	R_M16,
	// 8bit immediate data
	IMM8,
	// 16bit immediate data
	IMM16,
	// Like IMM8 but for relative jumps
	REL8,
	// Like IMM16 but for relative jumps
	REL16,
	// Special item just to mark the end
	END,
} addressing_mode_t;

typedef struct instruction {
	u_int8_t opcode;
	const char *name;
	addressing_mode_t mode[5];
	int extended;
	void (*exec)(state_t *state);
} instruction_t;

extern const instruction_t instructions[];
extern const instruction_t extended[][8];
extern const instruction_t invalid_instruction;

instruction_t parse_inst(u_int8_t *binary, unsigned long size);
unsigned get_inst_size(instruction_t inst, u_int8_t *binary, unsigned bin_size);
void print_instruction(unsigned addr, instruction_t inst, unsigned inst_size, u_int8_t *binary, bool space);

