#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dasm.h"

char *open_and_read(char *path)
{
	FILE *file = fopen(path, "r");
	if (!file) {
		dprintf(2, "Invalid path: %s. File does not exist.\n", path);
		return NULL;
	}

	int rc = fseek(file, 0L, SEEK_END);
	if (rc != 0) {
		fclose(file);
		return NULL;
	}
	long size = ftell(file);
	if (size == -1) {
		fclose(file);
		return NULL;
	}
	rewind(file);

	char *ret = malloc((size + 1) * sizeof(char));
	size = fread(ret, sizeof(char), size, file);
	ret[size] = '\0';

	fclose(file);
	return ret;
}

int main(int argc, char **argv)
{
	if (argc != 3 || strcmp(argv[1], "-d")) {
		printf("Usage: %s -d <binary>\n", argv[0]);
		return 2;
	}

	char *binary = open_and_read(argv[2]);
	if (!binary) {
		puts("Could not read binary file.");
		return 2;
	}

	int ret = dasm(binary);
	free(binary);
	return ret;
}
