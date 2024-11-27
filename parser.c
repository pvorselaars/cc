#include "parser.h"

void print_c_ast(c_ast_node_t * node)
{
	if (node == NULL)
		return;

	if (node->type == C_PROGRAM) {
		printf("PROGRAM(\n");
		print_c_ast(node->function);
		printf(")\n");
	} else if (node->type == C_FUNCTION) {
		printf("FUNCTION(\n");
		printf("name=\"%s\"\n", node->identifier);
		printf("body=");
		print_c_ast(node->body);
		printf(")\n");
	} else if (node->type == C_RETURN) {
		printf("Return(\n");
		print_c_ast(node->exp);
		printf(")\n");
	} else if (node->type == C_CONSTANT) {
		printf("Constant(%ld)\n", node->value);
	}
}

void free_c_ast(c_ast_node_t * node)
{
	if (node == NULL)
		return;

	if (node->type == C_PROGRAM) {
		free_c_ast(node->function);
	} else if (node->type == C_FUNCTION) {
		free(node->identifier);
		free_c_ast(node->body);
	} else if (node->type == C_RETURN) {
		free_c_ast(node->exp);
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
		return strdup(t->value);
	}

	return token_string(t->type);
}

c_ast_node_t *parse_c_expression(token_t ** tokens)
{
	c_ast_node_t *node = malloc(sizeof(c_ast_node_t));
	node->type = C_CONSTANT;

	char *val = expect(TOKEN_CONSTANT, NULL, tokens);
	node->value = strtol(val, NULL, 0);

	return node;
}

c_ast_node_t *parse_c_statement(token_t ** tokens)
{
	c_ast_node_t *node = malloc(sizeof(c_ast_node_t));
	node->type = C_RETURN;

	expect(TOKEN_KEYWORD, "return", tokens);
	node->exp = parse_c_expression(tokens);
	expect(TOKEN_SEMICOLON, NULL, tokens);

	return node;
}

c_ast_node_t *parse_c_function(token_t ** tokens)
{
	c_ast_node_t *node = malloc(sizeof(c_ast_node_t));
	node->type = C_FUNCTION;

	expect(TOKEN_KEYWORD, "int", tokens);
	node->identifier = expect(TOKEN_IDENTIFIER, NULL, tokens);

	expect(TOKEN_L_PARENTHESES, NULL, tokens);
	expect(TOKEN_KEYWORD, "void", tokens);
	expect(TOKEN_R_PARENTHESES, NULL, tokens);
	expect(TOKEN_L_BRACE, NULL, tokens);
	node->body = parse_c_statement(tokens);
	expect(TOKEN_R_BRACE, NULL, tokens);
	return node;
}

x64_ast_node_t *parse_x64_expression(c_ast_node_t * c_ast)
{
	x64_ast_node_t *node = malloc(sizeof(x64_ast_node_t));

	node->type = X64_IMM;
	node->value = c_ast->value;

	return node;
}

x64_ast_node_t *parse_x64_instructions(c_ast_node_t * c_ast)
{
	x64_ast_node_t *node = calloc(3, sizeof(x64_ast_node_t));
	x64_ast_node_t *reg = malloc(sizeof(x64_ast_node_t));

	reg->type = X64_REGISTER;

	node[0].type = X64_MOV;
	node[0].src = parse_x64_expression(c_ast->exp);
	node[0].dst = reg;

	node[1].type = X64_RET;

	return node;
}

x64_ast_node_t *parse_x64_function(c_ast_node_t * c_ast)
{
	x64_ast_node_t *node = malloc(sizeof(x64_ast_node_t));

	node->type = X64_FUNCTION;
	node->identifier = strdup(c_ast->identifier);
	node->instructions = parse_x64_instructions(c_ast->body);
	return node;
}

void print_x64_ast(x64_ast_node_t * node)
{
	if (node == NULL)
		return;

	if (node->type == X64_PROGRAM) {
		printf("PROGRAM(\n");
		print_x64_ast(node->function);
		printf(")\n");
	} else if (node->type == X64_FUNCTION) {
		printf("FUNCTION(\n");
		printf("name=\"%s\"\n", node->identifier);
		printf("body=");
		x64_ast_node_t *instruction = node->instructions;
		while (instruction->type != 0) {
			print_x64_ast(instruction++);
		}
		printf(")\n");
	} else if (node->type == X64_MOV) {
		printf("Mov(\n");
		printf("src=");
		print_x64_ast(node->src);
		printf(",\n");
		printf("dst=");
		print_x64_ast(node->dst);
		printf(")\n");
	} else if (node->type == X64_RET) {
		printf("Ret\n");
	} else if (node->type == X64_IMM) {
		printf("Imm(%ld)", node->value);
	} else if (node->type == X64_REGISTER) {
		printf("Register\n");
	}
}

x64_ast_node_t *parse_x64(c_ast_node_t * ast)
{

	x64_ast_node_t *AST = malloc(sizeof(x64_ast_node_t));
	AST->type = X64_PROGRAM;

	AST->function = parse_x64_function(ast->function);
	print_x64_ast(AST);

	return AST;
}

c_ast_node_t *parse(token_t * tokens)
{
	c_ast_node_t *AST = malloc(sizeof(c_ast_node_t));
	AST->type = C_PROGRAM;

	AST->function = parse_c_function(&tokens);

	print_c_ast(AST);

	return AST;
}
