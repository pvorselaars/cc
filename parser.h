#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
	C_PROGRAM,
	C_FUNCTION,
	C_RETURN,
	C_CONSTANT
} c_ast_node_type;

typedef struct c_ast_node_t {
	c_ast_node_type type;
	union {
		struct c_ast_node_t *function;
		struct {
			char *identifier;
			struct c_ast_node_t *body;
		};
		struct c_ast_node_t *exp;
		long value;
	};
} c_ast_node_t;

typedef enum {
	X64_NULL,
	X64_PROGRAM,
	X64_FUNCTION,
	X64_MOV,
	X64_RET,
	X64_REGISTER,
	X64_IMM
} x64_ast_node_type;

typedef struct x64_ast_node_t {
	x64_ast_node_type type;
	union {
		// root node
		struct x64_ast_node_t *function;
		// function
		struct {
			char *identifier;
			struct x64_ast_node_t *instructions;
		};
		// mov
		struct {
			struct x64_ast_node_t *src;
			struct x64_ast_node_t *dst;
			struct x64_ast_node_t *next;
		};
		struct x64_ast_node_t *exp;
		long value;
	};
} x64_ast_node_t;

c_ast_node_t * parse(token_t*);
x64_ast_node_t * parse_x64(c_ast_node_t*);
void free_c_ast(c_ast_node_t*);
#endif
