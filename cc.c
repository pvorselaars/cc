#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
		return 1;

	char lex     = 0;
	char parse   = 0;
	char codegen = 0;
	char *filename = NULL;

	for (int a = 1; a < argc; a++) {

		if (strcmp(argv[a], "--lex") == 0) {
			lex = 1;
			continue;
		}

		if (strcmp(argv[a], "--parse") == 0) {
			parse = 1;
			continue;
		}

		if (strcmp(argv[a], "--codegen") == 0) {
			codegen = 1;
			continue;
		}

		filename = argv[a];
	}

	if (!filename) {
		fprintf(stderr, "Usage: %s [options] <file>\n", argv[0]);
		return 1;
	}

	regex_t regex;
	if (regcomp(&regex, ".*\\.c$", REG_EXTENDED) != 0) {
		fprintf(stderr, "Unable to compile regex\n");
		return 1;
	}

	if (regexec(&regex, filename, 0, NULL, 0) != 0){
		fprintf(stderr, "%s is not a .c file\n", filename);
		return 1;
	}

	regfree(&regex);

	// Preprocess 
	
	char *fmt = "cc -E -P %s -o %s";
	char *cmd = malloc(sizeof(char) * (strlen(fmt) + 2*strlen(filename) - 4));
	char *out = malloc(sizeof(char) * strlen(filename));

	strcpy(out, filename);
	out[strlen(out)-1] = 'i';

	sprintf(cmd, fmt, filename, out);
	if (system(cmd) != 0)
		return 1;

	// Compile
	
	remove(out);

	if (lex) {
		return 0;
	}

	if (parse) {
		return 0;
	}

	if (codegen) {
		return 0;
	}
	// Assemble and link
	
	out[strlen(out)-1] = 's';
	fmt = "cc %s";
	sprintf(cmd, fmt, out);
	if (system(cmd) != 0)
		return 1;

	free(cmd);
	free(out);
	return 0;
}
	
