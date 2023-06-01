#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "dasm.h"

const char *registers8[] = { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" };
const char *registers16[] = { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di" };


void print_rm_operand(unsigned inst_size, u_int8_t *binary, bool is16bit)
{
	unsigned mod = binary[1] >> 6;
	unsigned rm = binary[1] & 0b111;
	char disp[20] = {0};

	if (mod == 0b11) {
		printf("%s", is16bit ? registers16[rm] : registers8[rm]);
		return;
	}

	if (mod == 0b10) {
		snprintf(disp, sizeof(disp), "+%04x", binary[inst_size - 1] | (binary[inst_size - 2] << 8));
	}
	if (mod == 0 && rm == 0b110) {
		snprintf(disp, sizeof(disp), "+%04x", (binary[inst_size - 1] << 8) | binary[inst_size - 2]);
	}
	if (mod == 0b01) {
		int8_t disp_v = binary[inst_size - 1];
		snprintf(disp, 20, "%c%x", disp_v < 0 ? '-' : '+', disp_v);
	}

	switch (rm)
	{
		case 0x00: printf("[bx+si%s]", disp); break;
		case 0x01: printf("[bx+di%s]", disp); break;
		case 0x02: printf("[bp+si%s]", disp); break;
		case 0x03: printf("[bp+di%s]", disp); break;
		case 0x04: printf("[si%s]", disp); break;
		case 0x05: printf("[di%s]", disp); break;
		case 0x06: printf("[bp%s]", disp); break;
		case 0x07: printf("[bx%s]", disp); break;
	}
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

void print_instruction(unsigned addr, instruction_t inst, unsigned inst_size, u_int8_t *binary)
{
	bool need_comma = strchr(inst.name, ' ');
	printf("%04x: %0*x%-*s %s", addr, inst_size * 2, read_size(binary, inst_size), 13 - inst_size * 2, "", inst.name);
	for (int i = 0; inst.mode[i] != END; i++) {
		if (need_comma)
			printf(", ");
		else
			printf(" ");
		need_comma = true;
		switch (inst.mode[i]) {
		case IMM8:
			printf("%02x", binary[inst_size - 1]);
			break;
		case IMM16:
			printf("%04x", (binary[inst_size - 1] << 8) | binary[inst_size - 2]);
			break;
		case REL8:
			printf("%02x", addr + inst_size - binary[inst_size - 1]);
			break;
		case REL16:
			printf("%04x", addr + inst_size - ((binary[inst_size - 1] << 8) | binary[inst_size - 2]));
			break;
		case REG8:
			printf("%s", registers8[(binary[1] & 0b111000) >> 3]);
			break;
		case REG16:
			printf("%s", registers16[(binary[1] & 0b111000) >> 3]);
			break;
		case R_M8:
			print_rm_operand(inst_size, binary, false);
			break;
		case R_M16:
			print_rm_operand(inst_size, binary, true);
			break;
		case END:
			break;
		}
	}
	printf("\n");
}

instruction_t parse_inst(u_int8_t *binary, unsigned long size)
{
	for (int i = 0; instructions[i].name; i++) {
		if (instructions[i].opcode != *binary)
			continue;

		if (instructions[i].extended == -1)
			return instructions[i];

		if (size < 2)
			return invalid_instruction;
		unsigned mod = binary[1] & 0b111000 >> 3;
		return extended[instructions[i].extended][mod];
	}
	return invalid_instruction;
}

unsigned get_inst_size(instruction_t inst, u_int8_t *binary, unsigned bin_size)
{
	unsigned size = 1;
	bool has_extended_mod = false;
	for (int i = 0; inst.mode[i] != END; i++) {
		switch(inst.mode[i]) {
		case IMM8:
		case REL8:
			size += 1;
			break;
		case IMM16:
		case REL16:
			size += 2;
			break;
		case R_M8:
		case R_M16: {
			if (bin_size < 2)
				return 2;
			unsigned mod = binary[1] >> 6;
			unsigned rm = binary[1] & 0b111;
			if (mod == 0b01)
				size++;
			else if (mod == 0b10 || (mod == 0 && rm == 0b110))
				size += 2;
			has_extended_mod = true;
			break;
		}
		case REG16:
		case REG8:
			has_extended_mod = true;
			break;
		case END:
			break;
		}
	}
	return size + has_extended_mod;
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
		instruction_t inst = parse_inst(binary, size - pc);
		unsigned inst_size = get_inst_size(inst, binary, size - pc);
		if (pc + inst_size > size) {
			printf("Invalid file. Missing operand for instruction: '%s'. (pc: %lx)\n", inst.name, pc);
			return 1;
		}
		print_instruction(pc, inst, inst_size, binary);
		pc += inst_size;
		binary += inst_size;
	}
	return 0;
}
