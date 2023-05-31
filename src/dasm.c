#include <sys/types.h>
#include <stdio.h>
#include "dasm.h"

void print_instruction(unsigned addr, instruction_t inst, unsigned raw)
{
	printf("%04x: %-14x %s", addr, raw, inst.name);
	for (int i = 0; inst.mode[i] != END; i++) {
		switch (inst.mode[i]) {
		case IMM8:
			printf(", %02x", raw & 0xFF);
			break;
		case IMM16:
			printf(", %04x", raw & 0xFFFF);
			break;
		default:
			printf(", undefined");
		}
	}
	printf("\n");
}

unsigned read_size(u_int8_t *binary, unsigned size)
{
	unsigned ret = binary[0];

	for (unsigned i = 1; i < size; i++) {
		ret <<= 8;
		ret |= binary[i];
	}
	return ret;
}

instruction_t parse_inst(u_int8_t *binary)
{
	for (int i = 0; instructions[i].name; i++) {
		if (instructions[i].opcode == *binary)
			return instructions[i];
	}
	return (const instruction_t){
		.opcode = 0xFF, .name = "unknown", .mode = {END}, .size = 1
	};
}

int dasm(u_int8_t *binary, unsigned long size)
{
	unsigned long pc = 0;
	int header_size = 0;

	if (binary[0] == 0xEB && binary[1] == 0x0E)
		header_size = 16;
	else if (binary[0] == 0x01 && binary[1] == 0x03)
		header_size = binary[4];

	binary += header_size;
	size -= header_size;

	while (pc < size) {
		instruction_t inst = parse_inst(binary);
		if (pc + inst.size > size) {
			printf("Invalid file. Missing operand for instruction: '%s'. (pc: %lx)\n", inst.name, pc);
			return 1;
		}
		unsigned long raw = read_size(binary, inst.size);
		print_instruction(pc, inst, raw);
		pc += inst.size;
		binary += inst.size;
	}
	return 0;
}
