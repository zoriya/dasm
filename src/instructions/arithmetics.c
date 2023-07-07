#include "../dasm.h"
#include "../interpretor.h"
#include <stdint.h>

void sub(const instruction_t *self, state_t *state)
{
	void *from = get_operand(self, 0, state);
	void *to = get_operand(self, 1, state);
	bool is16bit = is_operand_wide(self, 0);
	unsigned old;
	unsigned value;

	if (is16bit) {
		old = *(uint16_t *)from;
		value = old - *(uint16_t *)to;
		*(uint16_t *)from = value;
	} else {
		old = *(uint8_t *)from;
		value = old - *(uint8_t *)to;
		*(uint8_t *)from = value;
	}

	unsigned neg_mask = is16bit ? 0x8000 : 0x80;
	state->of = value != (is16bit ? *(uint16_t *)from : *(uint8_t *)from);
	state->cf = old < (is16bit ? *(uint16_t *)to : *(uint8_t *)to);
	state->sf = value & neg_mask;
	state->zf = value == 0;
	printf("Old %x - %x = %x\n", old, *(uint16_t *)to, *(uint16_t *)from);
	// TODO: Set AF and PF
}
