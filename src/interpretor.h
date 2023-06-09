#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MEMORY_SIZE 65536

typedef struct state {
	union {
		struct {
			uint8_t al;
			uint8_t ah;
		};
		uint16_t ax;
	};
	union {
		struct {
			uint8_t bl;
			uint8_t bh;
		};
		uint16_t bx;
	};
	union {
		struct {
			uint8_t cl;
			uint8_t ch;
		};
		uint16_t cx;
	};
	union {
		struct {
			uint8_t dl;
			uint8_t dh;
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
	unsigned binary_size;
	unsigned data_size;

	uint8_t memory[MEMORY_SIZE];

	struct {
		unsigned imm_idx;
		// A bunch of unsigneds to store temporary operands and return them as pointers.
		// instructions can't write to it, they are effectivly read-only.
		uint16_t operand_holder[5];
		bool debug;
	} parse_data;
} state_t;

typedef struct {
	uint16_t source;
	uint16_t type;
} syscall_t;

enum operand_type {
	BIT8,
	BIT16,
	BIT16_SMALL_ENDIAN,
};

typedef struct {
	uint8_t *ptr;
	enum operand_type type;
} operand_t;

