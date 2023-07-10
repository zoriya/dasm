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

	state->of = value != read_op(from);
	state->cf = old < minus;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set AF and PF
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
