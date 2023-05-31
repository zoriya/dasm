#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include "dasm.h"

const char *registers8[] = { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" };
const char *registers16[] = { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di" };


void print_rm_operand(instruction_t inst, unsigned raw, u_int8_t operand, bool is16bit)
{
	(void)inst;
	(void)raw;
	unsigned mod = operand << 6;
	unsigned rm = operand & 0b111;
	if (mod == 0b11) {
		printf(", %s", is16bit ? registers16[rm] : registers8[rm]);
		return;
	}
	// TODO: Understand and implement other modes here
}

void print_instruction(unsigned addr, instruction_t inst, unsigned raw)
{
	u_int8_t operand = raw & (0xFF << (inst.size - 2));
	printf("%04x: %0*x%-*s %s", addr, inst.size * 2, raw, 13 - inst.size * 2, "", inst.name);
	for (int i = 0; inst.mode[i] != END; i++) {
		switch (inst.mode[i]) {
		case IMM8:
			printf(", %02x", raw & 0xFF);
			break;
		case IMM16:
			printf(", %02x%02x", raw & 0xFF, (raw & 0xFF00) >> 8);
			break;
		case REG8:
			printf(", %s", registers8[operand & 0b111000 >> 3]);
			break;
		case REG16:
			printf(", %s", registers16[operand & 0b111000 >> 3]);
			break;
		case R_M8:
			print_rm_operand(inst, raw, operand, false);
			break;
		case R_M16:
			print_rm_operand(inst, raw, operand, true);
			break;
		case END:
			break;
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

	if (binary[0] == 0xEB && binary[1] == 0x0E) {
		header_size = 16;
		size = binary[2] | (binary[3] << 8);
	} else if (binary[0] == 0x01 && binary[1] == 0x03) {
		header_size = binary[4];
		size = binary[8]  | (binary[9] << 8)  | (binary[10] << 16) | (binary[11] << 24);
	}

	binary += header_size;

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
