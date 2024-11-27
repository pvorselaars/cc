#include "lexer.h"

static char* keywords[] = {
	"int",
	"void",
	"return",
};

static int num_keywords = sizeof(keywords)/sizeof(char*);

bool is_constant(const char *value)
{
	while (*value != 0) {
		if (!isdigit(*value))
			return false;

		value++;
	}

	return true;
}

bool is_keyword(const char *value)
{

	for (int i = 0; i < num_keywords; i++) {
		if (strcmp(keywords[i], value) == 0)
			return true;
	}

	return false;
}

token_t *get_token(char **input)
{
	token_t *token = malloc(sizeof(token_t));

	if (!token) {
		fprintf(stderr, "Unable to allocate token\n");
		return NULL;
	}

	token->value = NULL;
	token->next = NULL;

	char *position = *input;

	while (isspace(*position)) {
		position++;
	}

	if (*position == 0) {

		free(token);
		return NULL;

	} else if (isdigit(*position)) {

		int c = 0;
		char *str = position;
		while (*position != 0 && !isspace(*position) && *(position) != ';') {
			c++;
			position++;
		}

		token->value = malloc(sizeof(char) * c);
		strncpy(token->value, str, c);
		token->value[c] = '\0';

		if (is_constant(token->value)) {
			token->type = CONSTANT;
		} else {
			token->type = INVALID;
		}

	} else if (isalpha(*position)) {

		int c = 0;
		char *str = position;
		while (isalnum(*position)) {
			c++;
			position++;
		}

		token->value = malloc(sizeof(char) * c + 1);
		strncpy(token->value, str, c);
		token->value[c] = '\0';

		if (is_keyword(token->value)) {
			token->type = KEYWORD;
		} else {
			token->type = IDENTIFIER;
		}

	} else if (*position == '(') {
		token->type = L_PARENTHESES;
		position++;
	} else if (*position == ')') {
		token->type = R_PARENTHESES;
		position++;
	} else if (*position == '{') {
		token->type = L_BRACE;
		position++;
	} else if (*position == '}') {
		token->type = R_BRACE;
		position++;
	} else if (*position == ';') {
		token->type = SEMICOLON;
		position++;
	} else {
		token->type = INVALID;
		position++;
	}

	*input = position;
	return token;
}

void free_tokens(token_t * tokens)
{

	token_t *temp;
	while (tokens != NULL) {
		temp = tokens;
		tokens = tokens->next;

		if (temp->value != NULL) {
			free(temp->value);
		}
		free(temp);
	}
}

bool invalid_token(token_t * tokens)
{
	if (tokens == NULL)
		return false;

	while (tokens->type != INVALID) {

		if (tokens->next != NULL) {
			tokens = tokens->next;
		} else {
			return false;
		}
	}
	
	return true;
}

token_t *lex(FILE * stream)
{
	char *line;
	char *buf = NULL;
	size_t len = 0;
	ssize_t num_bytes;

	token_t *list = NULL;
	token_t *current = list;
	token_t *prev = list;

	while ((num_bytes = getline(&buf, &len, stream)) != -1) {
		line = buf;

		while ((current = get_token(&line)) != NULL) {

			if (list == NULL) {
				list = current;
			}

			if (prev != NULL) {
				prev->next = current;
			}

			if (current->type == INVALID) {
				fprintf(stderr, "Invalid token %s\n", current->value);
			}

			printf("%d: %s\n", current->type, current->value);

			prev = current;
		}

	}

	free(buf);

	return list;
}
