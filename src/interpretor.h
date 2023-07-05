#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct state {
	union {
		struct {
			uint8_t ah;
			uint8_t al;
		};
		uint16_t ax;
	};
	union {
		struct {
			uint8_t bh;
			uint8_t bl;
		};
		uint16_t bx;
	};
	union {
		struct {
			uint8_t ch;
			uint8_t cl;
		};
		uint16_t cx;
	};
	union {
		struct {
			uint8_t dh;
			uint8_t dl;
		};
		uint16_t dx;
	};
	uint16_t pc;
	uint16_t sp;
	uint16_t bp;
	uint16_t si;
	uint16_t di;

	union {
		struct {
			bool of:1;
			bool df:1;
			bool ifl:1;
			bool tf:1;
			bool sf:1;
			bool zf:1;
			bool nf:1;
			bool af:1;
			bool pf:1;
			bool cf:1;
		};
		unsigned flags:10;
	};

	uint8_t *binary;

	struct {
		unsigned imm_idx;
	} parse_data;
} state_t;
