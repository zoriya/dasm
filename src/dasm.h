int dasm(char *binary);

typedef struct instruction {
	const char *name;
	unsigned size;
} instruction_t;
