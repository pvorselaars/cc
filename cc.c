#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
		return 1;

	char *filename = argv[1];

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
	system(cmd);

	// Compile
	
	remove(out);

	// Assemble and link
	
	out[strlen(out)-1] = 's';
	fmt = "cc %s";
	sprintf(cmd, fmt, out);
	system(cmd);

	free(cmd);
	free(out);
	return 0;
}
	
