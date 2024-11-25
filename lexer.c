#include "lexer.h"

static int num_keywords = sizeof(keywords)/sizeof(char*);

int is_identifier(char *buffer)
{
	while (*buffer != 0) {
		if (!isalpha(*buffer))
			return 0;

		buffer++;
	}

	return 1;
}

int is_constant(char *buffer)
{
	while (*buffer != 0) {
		if (!isdigit(*buffer))
			return 0;

		buffer++;
	}

	return 1;
}

bool is_keyword(const char *value)
{

	for (int i = 0; i < num_keywords; i++) {
		if (strcmp(keywords[i], value))
			return true;
	}

	return false;
}

token_t get_token(char **input)
{
	token_t token;

	char *position = *input;

	while (isspace(*position)) {
		position++;
	}

	while (*position == '\n') {
		position++;
	}

	int i = 0;
	if (*position == 0) {

		token.type = END;
		token.value[i] = 0;

	} else if (isdigit(*position)) {


		while (isdigit(*position)) {
			token.value[i++] = *position++;
		}

		if (isalpha(*position)) {
			token.type = INVALID;
		} else {
			token.type = CONSTANT;
		}

		token.value[i] = 0;

	} else if (isalpha(*position)) {

		while (isalpha(*position)) {
			token.value[i++] = *position++;
		}

		if (is_keyword(token.value)) {
			token.type = KEYWORD;
		} else {
			token.type = INVALID;
		}

		token.value[i] = 0;

	} else if (*position == '(') {
		token.type = L_PARENTHESES;
		token.value[i] = 0;
		position++;
	} else if (*position == ')') {
		token.type = R_PARENTHESES;
		token.value[i] = 0;
		position++;
	} else if (*position == '{') {
		token.type = L_BRACE;
		token.value[i] = 0;
		position++;
	} else if (*position == '}') {
		token.type = R_BRACE;
		token.value[i] = 0;
		position++;
	} else if (*position == ';') {
		token.type = SEMICOLON;
		token.value[i] = 0;
		position++;
	} else {
		token.type = INVALID;
		token.value[i] = 0;
		position++;
	}

	*input = position;
	return token;
}

int lex(const char *filename)
{
	FILE *src = fopen(filename, "r");

	if (!src) {
		fprintf(stderr, "Unable to open source file %s\n", filename);
		return 1;
	}

	char *buf = malloc(sizeof(char)*LEX_BUFFER_SIZE+1);
	if (!buf) {
		fprintf(stderr, "Unable to allocate input buffer\n");
		return 1;
	}


	while (fread(buf, sizeof(char), LEX_BUFFER_SIZE, src)) {

		buf[LEX_BUFFER_SIZE] = 0;

		char *line = buf;

		token_t t = get_token(&line);

		while (t.type != END) {

			printf("%d: %s\n", t.type, t.value);

			if (t.type == INVALID)
				return 1;

			t = get_token(&line);
		}

	}
	
	free(buf);

	return 0;
}
