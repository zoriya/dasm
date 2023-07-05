#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "../dasm.h"

unsigned *get_reg_operand(state_t *state, bool is16bit)
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

	if (is16bit)
		return (void *)registers16[(state->binary[state->pc + 1] & 0b111000) >> 3];
	return (void *)registers8[(state->binary[state->pc + 1] & 0b111000) >> 3];
}

unsigned *get_rm_operand(const instruction_t *inst, state_t *state, unsigned *imm_idx, bool is16bit)
{
	unsigned mod = state->binary[1] >> 6;
	unsigned rm = state->binary[1] & 0b111;

	if (mod == 0b11)
		return get_reg_operand(state, is16bit);

	if (mod == 0 && rm == 0b110) {
		*imm_idx += 2;
		return (void *)&state->binary[state->pc + *imm_idx - 2];
	}
	// TODO:
	return NULL;
}

unsigned *get_operand(const instruction_t *inst, unsigned i, state_t *state)
{
	unsigned *ret = NULL;
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
		ret = get_reg_operand(state, false);
		break;
	case REG16:
		ret = get_reg_operand(state, true);
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

void mov(const instruction_t *self, state_t *state)
{
	unsigned *from = get_operand(self, 0, state);
	unsigned *to = get_operand(self, 1, state);

	printf("mov %p %p\n", from, to);
	printf("mov %x %x\n", (uint16_t)*from, (uint16_t)*to);
}
