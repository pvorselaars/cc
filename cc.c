#include "cc.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
		return 1;

	char lex_flag = 0;
	char parse_flag = 0;
	char codegen_flag = 0;
	char *filename = NULL;

	for (int a = 1; a < argc; a++) {

		if (strcmp(argv[a], "--lex") == 0) {
			lex_flag = 1;
			continue;
		}

		if (strcmp(argv[a], "--parse") == 0) {
			parse_flag = 1;
			continue;
		}

		if (strcmp(argv[a], "--codegen") == 0) {
			codegen_flag = 1;
			continue;
		}

		filename = argv[a];
	}

	if (!filename) {
		fprintf(stderr, "Usage: %s [options] <file>\n", argv[0]);
		return 1;
	}

	if (filename[strlen(filename) - 1] != 'c' && filename[strlen(filename) - 2] != '.') {
		fprintf(stderr, "%s is not a .c file\n", filename);
		return 1;
	}
	// Preprocess 

	char *fmt = "cc -E -P %s -o %s";
	char *cmd = malloc(sizeof(char) * (strlen(fmt) + 2 * strlen(filename) - 4));
	char *out = malloc(sizeof(char) * strlen(filename));

	strcpy(out, filename);
	out[strlen(out) - 1] = 'i';

	sprintf(cmd, fmt, filename, out);
	if (system(cmd) != 0)
		return 1;

	// Compile

	// Lex preprocessed file

	FILE *i = fopen(out, "r");

	token_t *list = lex(i);
	fclose(i);
	remove(out);

	if (invalid_token(list)) {
		free_tokens(list);
		return 1;
	}

	if (lex_flag) {
		free_tokens(list);
		return 0;
	}

	parse(list);
	free_tokens(list);
	if (parse_flag)
		return 0;

	if (codegen_flag)
		return 0;

	// Assemble and link

	out[strlen(out) - 1] = 's';
	fmt = "cc %s";
	sprintf(cmd, fmt, out);
	if (system(cmd) != 0)
		return 1;

	free(cmd);
	free(out);
	return 0;
}
