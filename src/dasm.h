#include <sys/types.h>

int dasm(u_int8_t *binary, unsigned long size);

// typedef enum addressing_mode {
// 	/// Param is the data (MOV AL, ~68FE~)
// 	IMMEDIATE,
// 	REGISTER,
// 	DIRECT,
// 	REGISTER_INDIRECT,
// 	BASED,
// 	INDEXED,
// 	BASED_INDEX,
// 	BASED_INDEX_DISPLACEMENT,
// } addressing_mode_t;

typedef enum addressing_mode {
	// 8bit register
	REG8,
	// 16bit register
	REG16,
	// Register or 8bit of memory
	R_M8,
	// Register or 16bit of memory
	R_M16,
	// 8bit immediate data
	IMM8,
	// 16bit immediate data
	IMM16,
	// Like IMM8 but for relative jumps
	REL8,
	// Like IMM16 but for relative jumps
	REL16,
	// Special item just to mark the end
	END,
} addressing_mode_t;

typedef struct instruction {
	u_int8_t opcode;
	const char *name;
	unsigned size;
	addressing_mode_t mode[5];
	unsigned extended;
} instruction_t;

extern const instruction_t instructions[];
extern const instruction_t extended[][8];
extern const instruction_t invalid_instruction;
