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
	char *out = strdup(filename);
	char *bin = strdup(filename);

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

	c_ast_node_t *c_ast = parse(list);
	free_tokens(list);
	if (parse_flag)
		return 0;

	x64_ast_node_t *x64_ast = parse_x64(c_ast);
	free_c_ast(c_ast);
	if (codegen_flag)
		return 0;

	out[strlen(out) - 1] = 's';
	FILE *s = fopen(out, "w");
	emit(s, x64_ast);
	fclose(s);

	// Assemble and link

	bin[strlen(bin) - 2] = 0;
	fmt = "cc %s -o %s";
	sprintf(cmd, fmt, out, bin);
	if (system(cmd) != 0)
		return 1;

	free(cmd);
	free(out);
	return 0;
}
