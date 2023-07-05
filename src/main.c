#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dasm.h"
#include "sys/types.h"

unsigned long open_and_read(char *path, u_int8_t **out)
{
	*out = NULL;
	FILE *file = fopen(path, "r");
	if (!file) {
		dprintf(2, "Invalid path: %s. File does not exist.\n", path);
		return 0;
	}

	int rc = fseek(file, 0L, SEEK_END);
	if (rc != 0) {
		fclose(file);
		return 0;
	}
	long size = ftell(file);
	if (size == -1) {
		fclose(file);
		return 0;
	}
	rewind(file);

	*out = malloc(size * sizeof(u_int8_t));
	size = fread(*out, sizeof(u_int8_t), size, file);
	fclose(file);
	return size;
}

int main(int argc, char **argv)
{
	if (argc != 3 || (strcmp(argv[1], "-d") && strcmp(argv[1], "-i"))) {
		printf("Usage:\n\tTo disassemble: %s -d <binary>\n\tTo interpret: %s -i <binary>\n", argv[0], argv[0]);
		return 2;
	}

	u_int8_t *binary;
	unsigned long size = open_and_read(argv[2], &binary);
	if (!binary) {
		puts("Could not read binary file.");
		return 2;
	}
	// Smaller than the header size.
	if (size < 32) {
		puts("Binary too small.");
		return 2;
	}

	int ret = strcmp(argv[1], "-i") ? dasm(binary, size) : interpret(binary, size);
	free(binary);
	return ret;
}
