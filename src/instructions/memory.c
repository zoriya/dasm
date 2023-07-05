#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "../dasm.h"

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
		return (void *)registers16[reg];
	return (void *)registers8[reg];
}

void *get_rm_operand(const instruction_t *inst, state_t *state, unsigned *imm_idx, bool is16bit)
{
	unsigned mod = state->binary[1] >> 6;
	unsigned rm = state->binary[1] & 0b111;

	if (mod == 0b11)
		return get_reg_operand(state, is16bit, false);

	if (mod == 0 && rm == 0b110) {
		*imm_idx += 2;
		return (void *)&state->binary[state->pc + *imm_idx - 2];
	}
	// TODO:
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
		imm_idx++;
		FALLTHROUGHT;
	case REL8:
		imm_idx++;
		ret = NULL;
		// TODO:
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
		ret = get_rm_operand(inst, state, &imm_idx, false);
		break;
	case R_M16:
		ret = get_rm_operand(inst, state, &imm_idx, true);
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

void mov(const instruction_t *self, state_t *state)
{
	void *from = get_operand(self, 0, state);
	void *to = get_operand(self, 1, state);

	if (is_operand_wide(self, 0))
		*(uint16_t *)from = *(uint16_t *)to;
	else
		*(uint8_t *)from = *(uint8_t *)to;
}
