#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LEX_BUFFER_SIZE 512

typedef enum {
	INVALID,
	IDENTIFIER,
	KEYWORD,
	CONSTANT,
	L_PARENTHESES,
	R_PARENTHESES,
	L_BRACE,
	R_BRACE,
	SEMICOLON,
	END
} token_type;

typedef struct token_t {
	token_type type;
	char *value;
	struct token_t *next;
} token_t;

void free_tokens(token_t*);
bool invalid_token(token_t*);
token_t *lex(FILE*);

#endif
