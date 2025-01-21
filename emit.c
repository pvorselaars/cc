#include "emit.h"

void emit(FILE *stream, x64_ast_node_t *ast)
{
	if (ast->type == X64_PROGRAM) {
		emit(stream, ast->function);
	}

	if (ast->type == X64_FUNCTION) {
		fprintf(stream, "\t.globl %s\n", ast->identifier);
		fprintf(stream, "%s:\n", ast->identifier);
		x64_ast_node_t *instruction = ast->instructions;
		while (instruction->type != 0) {
			emit(stream, instruction++);
		}
	}

	if (ast->type == X64_MOV) {
		fprintf(stream, "\tmovl ");
		emit(stream, ast->src);
		fprintf(stream, ", ");
		emit(stream, ast->dst);
		fprintf(stream, "\n");
	}

	if (ast->type == X64_RET) {
		fprintf(stream, "\tret\n");
	}

	if (ast->type == X64_REGISTER) {
		fprintf(stream, "%%eax ");
	}

	if (ast->type == X64_IMM) {
		fprintf(stream, "$%ld ", ast->value);
	}
}
