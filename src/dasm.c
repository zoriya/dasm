#include <sys/types.h>
#include <stdio.h>
#include "dasm.h"

void print_instruction(unsigned addr, instruction_t inst, unsigned raw)
{
	printf("%04x: %-14x %s\n", addr, raw, inst.name);
}

unsigned read_size(u_int8_t *binary, unsigned size)
{
	unsigned ret = binary[0];

	for (unsigned i = 1; i < size; i++) {
		ret |= binary[i] << (8 * i);
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
	int pc = 0;
	int header_size = 0;

	if (binary[0] == 0xEB && binary[1] == 0x0E)
		header_size = 16;
	else if (binary[0] == 0x01 && binary[1] == 0x03)
		header_size = binary[4];

	printf("Header size: %d\n", header_size);
	binary += header_size;

	while (pc < size - header_size) {
		instruction_t inst = parse_inst(binary);
		unsigned long raw = read_size(binary, inst.size);
		print_instruction(pc, inst, raw);
		pc += inst.size;
		binary += inst.size;
	}
	return 0;
}
