#include "../dasm.h"
#include "../interpretor.h"
#include <stdint.h>

void add(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	operand_t to = get_operand(self, 1, state);
	unsigned old = read_op(from);
	unsigned minus = read_op(to);
	unsigned value = old + minus;

	write_op(from, value);

	state->of = value != read_op(from);
	state->cf = value >= (is_operand_wide(self, 0) ? 0x10000 : 0x100);
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set AF and PF
}

void sub(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	operand_t to = get_operand(self, 1, state);
	unsigned old = read_op(from);
	unsigned minus = read_op(to);
	unsigned value = old - minus;

	write_op(from, value);

	// FIXME: value != read_op(from);
	state->of = false;
	state->cf = old < minus;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set AF and PF
}

void div(const instruction_t *self, state_t *state)
{
	operand_t to = get_operand(self, 0, state);
	unsigned old;
	unsigned minus = read_op(to);

	if (is_operand_wide(self, 0)) {
		old = (state->dx << 16) | state->ax;
		state->ax = old / minus;
		state->dx = old % minus;
	} else {
		old = state->ax;
		state->al = old / minus;
		state->ah = old % minus;
	}
}

void and_inst(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	operand_t to = get_operand(self, 1, state);
	unsigned old = read_op(from);
	unsigned minus = read_op(to);
	unsigned value = old & minus;

	write_op(from, value);

	state->of = 0;
	state->cf = 0;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set PF
}

void or_inst(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	operand_t to = get_operand(self, 1, state);
	unsigned old = read_op(from);
	unsigned minus = read_op(to);
	unsigned value = old | minus;

	write_op(from, value);

	state->of = 0;
	state->cf = 0;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set PF
}

void xor_inst(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	operand_t to = get_operand(self, 1, state);
	unsigned old = read_op(from);
	unsigned minus = read_op(to);
	unsigned value = old ^ minus;

	write_op(from, value);

	state->of = 0;
	state->cf = 0;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set PF
}

void cmp(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	operand_t to = get_operand(self, 1, state);
	unsigned old = read_op(from);
	unsigned minus = read_op(to);
	unsigned value = old - minus;
	// unsigned smallv = is_operand_wide(self, 0) ? (uint16_t)value : (uint8_t)value;

	// FIXME: The flag should be set according to value != smallv but the reference is buggy and always clear the overflow flag.
	state->of = 0; // value != smallv;
	state->cf = old < minus;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set AF and PF
}

void test(const instruction_t *self, state_t *state)
{
	operand_t from = self->opcode == 0xA8 || self->opcode == 0xA9
		? (operand_t){.ptr = &state->al, .type = self->opcode == 0xA8 ? BIT8 : BIT16_SMALL_ENDIAN}
		: get_operand(self, 0, state);
	operand_t to = get_operand(self, self->mode[1] == END ? 0 : 1, state);
	unsigned old = read_op(from);
	unsigned minus = read_op(to);
	unsigned value = old & minus;

	state->of = 0;
	state->cf = 0;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set PF
}

void inc(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	unsigned value = read_op(from) + 1;

	write_op(from, value);

	// FIXME: of should be set to value != read_op(from);
	state->of = 0;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set AF and PF
}

void dec(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	unsigned value = read_op(from) - 1;

	write_op(from, value);

	// FIXME: of should be set to value != read_op(from);
	state->of = 0;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set AF and PF
}

void cbw(const instruction_t *self, state_t *state)
{
	(void)self;
	state->ah = state->al >> 0x7 & 1 ? 0xFF : 0;
}

void cwd(const instruction_t *self, state_t *state)
{
	(void)self;
	state->dx = state->ah >> 0x7 & 1 ? 0xFFFF : 0;
}

void neg(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	unsigned value = -read_op(from);

	write_op(from, value);

	state->cf = value != 0;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
}

void shl(const instruction_t *self, state_t *state)
{
	operand_t to = get_operand(self, 0, state);
	unsigned value = read_op(to);
	uint8_t opgrp =state->binary[state->pc];
	unsigned shft = (opgrp == 0xd0 || opgrp == 0xd1) ? 1 : state->cl;
	unsigned new = value << shft;

	write_op(to, new);

	state->of = new != read_op(to);
	state->cf = value >> (8 - shft) & 1;
	state->sf = new & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = new == 0;
}

void sar(const instruction_t *self, state_t *state)
{
	operand_t to = get_operand(self, 0, state);
	unsigned value = read_op(to);
	uint8_t opgrp =state->binary[state->pc];
	unsigned shft = (opgrp == 0xd0 || opgrp == 0xd1) ? 1 : state->cl;

	unsigned signMask = (is_operand_wide(self, 0) ? 0x8000 : 0x80);

	unsigned new = value >> shft;
	if (value & signMask)
		new |= signMask;
	else
		new &= ~signMask;

	write_op(to, new);

	state->cf = (value >> (shft - 1)) & 1;
	state->sf = value & signMask;
	state->zf = new == 0;
}

void xchg(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	operand_t to = get_operand(self, 1, state);

	unsigned tmp = read_op(from);
	write_op(from, read_op(to));
	write_op(to, tmp);
}
