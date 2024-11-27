#include "parser.h"

typedef enum {
	PROGRAM,
	FUNCTION,
	RETURN,
	CONSTANT
} ast_node_type;

typedef struct ast_node_t {
	ast_node_type type;
	union {
		struct ast_node_t *function;
		struct {
			char *identifier;
			struct ast_node_t *body;
		};
		struct ast_node_t *exp;
		long value;
	};
} ast_node_t;

void print_ast(ast_node_t * node)
{
	if (node == NULL)
		return;

	if (node->type == PROGRAM) {
		printf("PROGRAM(\n");
		print_ast(node->function);
		printf(")\n");
	}

	if (node->type == FUNCTION) {
		printf("FUNCTION(\n");
		printf("name=\"%s\"\n", node->identifier);
		printf("body=");
		print_ast(node->body);
		printf(")\n");
	}

	if (node->type == RETURN) {
		printf("Return(\n");
		print_ast(node->exp);
		printf(")\n");
	}

	if (node->type == CONSTANT) {
		printf("Constant(%ld)\n", node->value);
	}
}

char *expect(token_type type, char *token, token_t ** tokens)
{
	token_t *t = take_token(tokens);

	if (t->type != type) {
		if (token != NULL && t->value != NULL) {
			fprintf(stderr, "Syntax error. Expected %s \"%s\", found \"%s\"\n", token_string(type), token,
				t->value);
		} else if (token == NULL && t->value != NULL) {
			fprintf(stderr, "Syntax error. Expected \"%s\", found \"%s\"\n", token_string(type), t->value);
		} else if (token == NULL && t->value == NULL) {
			fprintf(stderr, "Syntax error. Expected \"%s\", found \"%s\"\n", token_string(type),
				token_string(t->type));
		}
		exit(1);
	}

	if (t->value != NULL) {
		return t->value;
	}

	return token_string(t->type);
}

ast_node_t *parse_expression(token_t ** tokens)
{
	ast_node_t *node = malloc(sizeof(ast_node_t));
	node->type = CONSTANT;

	char *val = expect(TOKEN_CONSTANT, NULL, tokens);
	node->value = strtol(val, NULL, 0);

	return node;
}

ast_node_t *parse_statement(token_t ** tokens)
{
	ast_node_t *node = malloc(sizeof(ast_node_t));
	node->type = RETURN;

	expect(TOKEN_KEYWORD, "return", tokens);
	node->exp = parse_expression(tokens);
	expect(TOKEN_SEMICOLON, NULL, tokens);

	return node;
}

ast_node_t *parse_function(token_t ** tokens)
{
	ast_node_t *node = malloc(sizeof(ast_node_t));
	node->type = FUNCTION;

	expect(TOKEN_KEYWORD, "int", tokens);
	node->identifier = expect(TOKEN_IDENTIFIER, NULL, tokens);

	expect(TOKEN_L_PARENTHESES, NULL, tokens);
	expect(TOKEN_KEYWORD, "void", tokens);
	expect(TOKEN_R_PARENTHESES, NULL, tokens);
	expect(TOKEN_L_BRACE, NULL, tokens);
	node->body = parse_statement(tokens);
	expect(TOKEN_R_BRACE, NULL, tokens);
	return node;
}

int parse(token_t * tokens)
{
	ast_node_t AST;
	AST.type = PROGRAM;

	AST.function = parse_function(&tokens);

	print_ast(&AST);

	return 0;
}
