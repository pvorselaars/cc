#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {
	TOKEN_INVALID,
	TOKEN_IDENTIFIER,
	TOKEN_KEYWORD,
	TOKEN_CONSTANT,
	TOKEN_L_PARENTHESES,
	TOKEN_R_PARENTHESES,
	TOKEN_L_BRACE,
	TOKEN_R_BRACE,
	TOKEN_SEMICOLON
} token_type;

typedef struct token_t {
	token_type type;
	char *value;
	struct token_t *next;
} token_t;

char *token_string(token_type type);
void free_tokens(token_t*);
bool invalid_token(token_t*);
token_t *lex(FILE*);
token_t *take_token(token_t**);

#endif
