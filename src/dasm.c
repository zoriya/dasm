#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "dasm.h"
#include "instructions.h"

const char *registers8[] = { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" };
const char *registers16[] = { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di" };


int print_rm_operand(u_int8_t *binary, int imm_idx, bool is16bit)
{
	unsigned mod = binary[1] >> 6;
	unsigned rm = binary[1] & 0b111;
	char disp[20] = {0};

	if (mod == 0b11) {
		printf("%s", is16bit ? registers16[rm] : registers8[rm]);
		return 0;
	}

	if (mod == 0 && rm == 0b110) {
		printf("[%04x]", (binary[imm_idx + 1] << 8) | binary[imm_idx]);
		return 2;
	}

	int imm_offset = 0;

	if (mod == 0b10) {
		snprintf(disp, sizeof(disp), "+%x", (binary[imm_idx + 1] << 8) | binary[imm_idx]);
		imm_offset += 2;
	}
	if (mod == 0b01) {
		int8_t disp_v = binary[imm_idx];
		imm_offset++;
		snprintf(disp, sizeof(disp), "%c%x", disp_v < 0 ? '-' : '+', disp_v < 0 ? -disp_v : disp_v);
	}

	switch (rm) {
		case 0x00: printf("[bx+si%s]", disp); break;
		case 0x01: printf("[bx+di%s]", disp); break;
		case 0x02: printf("[bp+si%s]", disp); break;
		case 0x03: printf("[bp+di%s]", disp); break;
		case 0x04: printf("[si%s]", disp); break;
		case 0x05: printf("[di%s]", disp); break;
		case 0x06: printf("[bp%s]", disp); break;
		case 0x07: printf("[bx%s]", disp); break;
	}
	return imm_offset;
}

unsigned long read_size(u_int8_t *binary, unsigned size)
{
	unsigned long ret = binary[0];

	for (unsigned i = 1; i < size; i++) {
		ret <<= 8;
		ret |= binary[i];
	}
	return ret;
}

bool has_reg(const instruction_t *inst)
{
	for (int i = 0; inst->mode[i] != END; i++) {
		switch (inst->mode[i]) {
		case REG8:
		case REG16:
		case R_M8:
		case R_M16:
			return true;
		case OPREG8:
		case OPREG16:
		case IMM8:
		case IMM16:
		case REL8:
		case REL16:
		case END:
			continue;
		}
	}
	return false;
}

bool is_byte_operand(const instruction_t *inst, uint8_t *binary)
{
	if (inst->exec != &test || is_operand_wide(inst, 0))
		return false;
	for (int i = 0; inst->mode[i] != END; i++) {
		if (inst->mode[i] == REG8 || inst->mode[i] == REG16)
			return false;
		if (
			(inst->mode[i] == R_M8 || inst->mode[i] == R_M16)
			&& binary[1] >> 6 == 0b11
		)
			return false;

	}
	return true;
}

void print_instruction(unsigned addr, instruction_t inst, unsigned inst_size, u_int8_t *binary, bool space)
{
	char *last_param = strchr(inst.name, '%');
	// if the instruction has already a param in it (ex `in al`), we need to directly add a comma.
	// `jmp short` is an exception as it is the only instruction with a space in it that is not a parameter.
	bool need_comma = !last_param && strchr(inst.name, ' ') && inst.opcode != 0xEB;
	int imm_idx = 1 + (inst.extended != -1 || has_reg(&inst));

	printf("%04x:%s%0*lx%-*s", addr, space ? " " : "", inst_size * 2, read_size(binary, inst_size), 14 - inst_size * 2 - !space, "");
	if (last_param)
		printf("%.*s", (int)(last_param - inst.name - 1), inst.name);
	else
		printf("%s", inst.name);
	if (is_byte_operand(&inst, binary)) {
		printf(" byte");
	}

	for (int i = 0; inst.mode[i] != END; i++) {
		if (need_comma)
			printf(", ");
		else
			printf(" ");
		need_comma = true;
		switch (inst.mode[i]) {
		case IMM8:
			printf("%02x", binary[imm_idx++]);
			break;
		case IMM16:
			printf("%04x", (binary[imm_idx + 1] << 8) | binary[imm_idx]);
			imm_idx += 2;
			break;
		case REL8:
			printf("%04x", addr + inst_size + (int8_t)binary[imm_idx++]);
			break;
		case REL16:
			printf("%04x", addr + inst_size + (int16_t)((binary[imm_idx + 1] << 8) | binary[imm_idx]));
			imm_idx += 2;
			break;
		case REG8:
			printf("%s", registers8[(binary[1] & 0b111000) >> 3]);
			break;
		case REG16:
			printf("%s", registers16[(binary[1] & 0b111000) >> 3]);
			break;
		case OPREG8:
			printf("%s", registers8[binary[0] & 0b111]);
			break;
		case OPREG16:
			printf("%s", registers16[binary[0] & 0b111]);
			break;
		case R_M8:
			imm_idx += print_rm_operand(binary, imm_idx, false);
			break;
		case R_M16:
			imm_idx += print_rm_operand(binary, imm_idx, true);
			break;
		case END:
			break;
		}
	}
	if (last_param)
		printf("%s", last_param + 2);
	if (space)
		printf("\n");
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
		case OPREG8:
		case OPREG16:
		case END:
			break;
		}
	}
	return size + has_extended_mod;
}

int dasm(u_int8_t *binary, unsigned long size)
{
	unsigned pc = 0;
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
			printf("%04x: %02x            (undefined)\n", pc, inst.opcode);
			return 0;
		}
		print_instruction(pc, inst, inst_size, binary, true);
		pc += inst_size;
		binary += inst_size;
	}
	return 0;
}
