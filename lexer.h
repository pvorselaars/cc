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

typedef struct {
	token_type type;
	char value[32];
} token_t;

static char* keywords[] = {
	"int",
	"void",
	"return",
};

int lex(const char*);

#endif
