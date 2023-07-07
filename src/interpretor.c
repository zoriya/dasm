#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include "dasm.h"
#include "interpretor.h"

void *get_reg_operand(state_t *state, bool is16bit, bool is_in_op)
{
	uint8_t *registers8[8] = {
		&state->al,
		&state->cl,
		&state->dl,
		&state->bl,
		&state->ah,
		&state->ch,
		&state->dh,
		&state->bh,
	};
	uint16_t *registers16[8] = {
		&state->ax,
		&state->cx,
		&state->dx,
		&state->bx,
		&state->sp,
		&state->bp,
		&state->si,
		&state->di,
	};

	unsigned reg = is_in_op
		? state->binary[state->pc] & 0b111
		: (state->binary[state->pc + 1] & 0b111000) >> 3;
	if (is16bit)
		return registers16[reg];
	return registers8[reg];
}

void *get_rm_operand(state_t *state, unsigned *imm_idx, bool is16bit)
{
	unsigned mod = state->binary[state->pc + 1] >> 6;
	unsigned rm = state->binary[state->pc + 1] & 0b111;

	if (mod == 0b11)
		return get_reg_operand(state, is16bit, false);

	if (mod == 0 && rm == 0b110) {
		*imm_idx += 2;
		return state->memory + state->binary[state->pc + state->parse_data.imm_idx];
	}
	int disp = mod == 0b10
		? *(uint16_t*)&state->binary[state->pc + state->parse_data.imm_idx]
		: (int8_t)state->binary[state->pc + state->parse_data.imm_idx];
	state->parse_data.imm_idx += mod == 0b10 ? 2 : 1;

	switch (rm) {
	case 0x00:
		return state->memory + state->bx + state->si + disp;
	case 0x01:
		return state->memory + state->bx + state->di + disp;
	case 0x02:
		return state->memory + state->bp + state->si + disp;
	case 0x03:
		return state->memory + state->bp + state->di + disp;
	case 0x04:
		return state->memory + state->si + disp;
	case 0x05:
		return state->memory + state->di + disp;
	case 0x06:
		return state->memory + state->bp + disp;
	case 0x07:
		return state->memory + state->bx + disp;
	}
	return NULL;
}

void *get_operand(const instruction_t *inst, unsigned i, state_t *state)
{
	void *ret = NULL;
	unsigned imm_idx = 0;

	switch (inst->mode[i]) {
	case IMM16:
		imm_idx++;
		FALLTHROUGHT;
	case IMM8:
		imm_idx++;
		ret = (void *)&state->binary[state->pc + state->parse_data.imm_idx];
		break;
	case REL16:
		imm_idx += 2;
		state->parse_data.operand_holder[i] = state->pc
				+ get_inst_size(*inst, state->binary + state->pc, state->binary_size - state->pc)
				+ (int16_t)state->binary[state->pc + state->parse_data.imm_idx];
		ret = &state->parse_data.operand_holder[i];
		break;
	case REL8:
		imm_idx++;
		state->parse_data.operand_holder[i] = state->pc
				+ get_inst_size(*inst, state->binary + state->pc, state->binary_size - state->pc)
				+ (int8_t)state->binary[state->pc + state->parse_data.imm_idx];
		ret = &state->parse_data.operand_holder[i];
		break;
	case REG8:
		ret = get_reg_operand(state, false, false);
		break;
	case REG16:
		ret = get_reg_operand(state, true, false);
		break;
	case OPREG8:
		ret = get_reg_operand(state, false, true);
		break;
	case OPREG16:
		ret = get_reg_operand(state, true, true);
		break;
	case R_M8:
		ret = get_rm_operand(state, &imm_idx, false);
		break;
	case R_M16:
		ret = get_rm_operand(state, &imm_idx, true);
		break;
	case END:
		printf("Invalid parameter read.");
		break;
	}
	state->parse_data.imm_idx += imm_idx;
	return ret;
}

bool is_operand_wide(const instruction_t *inst, unsigned i)
{
	switch (inst->mode[i]) {
	case REG8:
	case OPREG8:
	case R_M8:
	case IMM8:
	case REL8:
		return false;
	case REG16:
	case OPREG16:
	case R_M16:
	case IMM16:
	case REL16:
	case END:
		return true;
	}
	return true;
}

void print_state(state_t *state)
{
	printf(
		"%04x %04x %04x %04x %04x %04x %04x %04x %c%c%c%c ",
		state->ax,
		state->bx,
		state->cx,
		state->dx,
		state->sp,
		state->bp,
		state->si,
		state->di,
		state->of ? 'O' : '-',
		state->sf ? 'S' : '-',
		state->zf ? 'Z' : '-',
		state->cf ? 'C' : '-'
	);
}

int interpret(u_int8_t *binary, unsigned long size)
{
	state_t *state = calloc(sizeof(*state), 1);
	int header_size = 0;
	int dsize= 0;

	state->sp = 0xFFDC;

	if (binary[0] == 0xEB && binary[1] == 0x0E) {
		header_size = 16;
		size = binary[2] | (binary[3] << 8);
		dsize = binary[4] | (binary[5] << 8);
	} else if (binary[0] == 0x01 && binary[1] == 0x03) {
		header_size = binary[4];
		size = binary[8]  | (binary[9] << 8)  | (binary[10] << 16) | (binary[11] << 24);
		dsize = binary[12] | (binary[13] << 8) | (binary[14] << 16) | (binary[15] << 24);
	}

	binary += header_size;
	state->binary = binary;
	state->binary_size = size;

	memcpy(state->memory, binary + size, dsize);

	printf(" AX   BX   CX   DX   SP   BP   SI   DI  FLAGS IP\n");
	while (state->pc < size) {
		instruction_t inst = parse_inst(binary, size - state->pc);
		unsigned inst_size = get_inst_size(inst, binary, size - state->pc);
		if (state->pc + inst_size > size) {
			return 0;
		}
		print_state(state);
		print_instruction(state->pc, inst, inst_size, binary, false);
		state->parse_data.imm_idx = 1 + (inst.extended != -1 || has_reg(&inst));
		if (inst.exec)
			inst.exec(&inst, state);
		else
			printf("Not implemented.\n");
		state->pc += inst_size;
		binary += inst_size;
	}
	return 0;
}
