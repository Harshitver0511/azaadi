#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode *ast_create_node(ASTNodeType type) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    node->type = type;
    return node;
}

ASTNode *ast_create_program() {
    ASTNode *node = ast_create_node(AST_PROGRAM);
    node->program.functions = NULL;
    node->program.function_count = 0;
    return node;
}

ASTNode *ast_create_function(char *name, DataType ret_type, ASTNode *body) {
    ASTNode *node = ast_create_node(AST_FUNCTION);
    node->function.name = strdup(name);
    node->function.return_type = ret_type;
    node->function.body = body;
    node->function.params = NULL;
    node->function.param_count = 0;
    return node;
}

ASTNode *ast_create_block() {
    ASTNode *node = ast_create_node(AST_BLOCK);
    node->block.statements = NULL;
    node->block.statement_count = 0;
    return node;
}

ASTNode *ast_create_var_decl(char *name, DataType type, ASTNode *init) {
    ASTNode *node = ast_create_node(AST_VAR_DECL);
    node->var_decl.name = strdup(name);
    node->var_decl.var_type = type;
    node->var_decl.init = init;
    return node;
}

ASTNode *ast_create_if(ASTNode *cond, ASTNode *then_br, ASTNode *else_br) {
    ASTNode *node = ast_create_node(AST_IF);
    node->if_stmt.condition = cond;
    node->if_stmt.then_branch = then_br;
    node->if_stmt.else_branch = else_br;
    return node;
}

ASTNode *ast_create_while(ASTNode *cond, ASTNode *body) {
    ASTNode *node = ast_create_node(AST_WHILE);
    node->while_stmt.condition = cond;
    node->while_stmt.body = body;
    return node;
}

ASTNode *ast_create_return(ASTNode *value) {
    ASTNode *node = ast_create_node(AST_RETURN);
    node->return_stmt.value = value;
    return node;
}

ASTNode *ast_create_binary_op(char *op, ASTNode *left, ASTNode *right) {
    ASTNode *node = ast_create_node(AST_BINARY_OP);
    node->binary_op.op = strdup(op);
    node->binary_op.left = left;
    node->binary_op.right = right;
    return node;
}

ASTNode *ast_create_assign(char *name, ASTNode *value) {
    ASTNode *node = ast_create_node(AST_ASSIGN);
    node->assign.name = strdup(name);
    node->assign.value = value;
    return node;
}

ASTNode *ast_create_call(char *name, ASTNode **args, int arg_count) {
    ASTNode *node = ast_create_node(AST_CALL);
    node->call.name = strdup(name);
    node->call.args = args;
    node->call.arg_count = arg_count;
    return node;
}

ASTNode *ast_create_variable(char *name) {
    ASTNode *node = ast_create_node(AST_VARIABLE);
    node->variable.name = strdup(name);
    return node;
}

ASTNode *ast_create_number(double value) {
    ASTNode *node = ast_create_node(AST_NUMBER);
    node->number.value = value;
    return node;
}

void ast_add_statement(ASTNode *block, ASTNode *stmt) {
    block->block.statement_count++;
    block->block.statements = realloc(block->block.statements, 
        sizeof(ASTNode*) * block->block.statement_count);
    block->block.statements[block->block.statement_count - 1] = stmt;
}

void ast_add_function(ASTNode *program, ASTNode *func) {
    program->program.function_count++;
    program->program.functions = realloc(program->program.functions,
        sizeof(ASTNode*) * program->program.function_count);
    program->program.functions[program->program.function_count - 1] = func;
}

void ast_free(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_PROGRAM:
            for (int i = 0; i < node->program.function_count; i++) {
                ast_free(node->program.functions[i]);
            }
            free(node->program.functions);
            break;
        case AST_FUNCTION:
            free(node->function.name);
            ast_free(node->function.body);
            for (int i = 0; i < node->function.param_count; i++) {
                ast_free(node->function.params[i]);
            }
            free(node->function.params);
            break;
        case AST_BLOCK:
            for (int i = 0; i < node->block.statement_count; i++) {
                ast_free(node->block.statements[i]);
            }
            free(node->block.statements);
            break;
        case AST_VAR_DECL:
            free(node->var_decl.name);
            ast_free(node->var_decl.init);
            break;
        case AST_BINARY_OP:
            free(node->binary_op.op);
            ast_free(node->binary_op.left);
            ast_free(node->binary_op.right);
            break;
        case AST_ASSIGN:
            free(node->assign.name);
            ast_free(node->assign.value);
            break;
        case AST_VARIABLE:
            free(node->variable.name);
            break;
        default:
            break;
    }
    
    free(node);
}