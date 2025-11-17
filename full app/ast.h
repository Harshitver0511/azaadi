#ifndef AST_H
#define AST_H

typedef enum {
    AST_PROGRAM,
    AST_FUNCTION,
    AST_BLOCK,
    AST_VAR_DECL,
    AST_IF,
    AST_WHILE,
    AST_FOR,
    AST_RETURN,
    AST_BREAK,
    AST_CONTINUE,
    AST_EXPR_STMT,
    AST_BINARY_OP,
    AST_UNARY_OP,
    AST_ASSIGN,
    AST_CALL,
    AST_VARIABLE,
    AST_NUMBER,
    AST_STRING
} ASTNodeType;

typedef enum {
    TYPE_INT,
    TYPE_CHAR,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_VOID
} DataType;

typedef struct ASTNode {
    ASTNodeType type;
    DataType data_type;
    
    union {
        struct {
            struct ASTNode **functions;
            int function_count;
        } program;
        
        struct {
            char *name;
            DataType return_type;
            struct ASTNode **params;
            int param_count;
            struct ASTNode *body;
        } function;
        
        struct {
            struct ASTNode **statements;
            int statement_count;
        } block;
        
        struct {
            char *name;
            DataType var_type;
            struct ASTNode *init;
        } var_decl;
        
        struct {
            struct ASTNode *condition;
            struct ASTNode *then_branch;
            struct ASTNode *else_branch;
        } if_stmt;
        
        struct {
            struct ASTNode *condition;
            struct ASTNode *body;
        } while_stmt;
        
        struct {
            struct ASTNode *init;
            struct ASTNode *condition;
            struct ASTNode *update;
            struct ASTNode *body;
        } for_stmt;
        
        struct {
            struct ASTNode *value;
        } return_stmt;
        
        struct {
            struct ASTNode *expr;
        } expr_stmt;
        
        struct {
            char *op;
            struct ASTNode *left;
            struct ASTNode *right;
        } binary_op;
        
        struct {
            char *op;
            struct ASTNode *operand;
        } unary_op;
        
        struct {
            char *name;
            struct ASTNode *value;
        } assign;
        
        struct {
            char *name;
            struct ASTNode **args;
            int arg_count;
        } call;
        
        struct {
            char *name;
        } variable;
        
        struct {
            double value;
        } number;
        
        struct {
            char *value;
        } string;
    };
} ASTNode;

ASTNode *ast_create_program();
ASTNode *ast_create_function(char *name, DataType ret_type, ASTNode *body);
ASTNode *ast_create_block();
ASTNode *ast_create_var_decl(char *name, DataType type, ASTNode *init);
ASTNode *ast_create_if(ASTNode *cond, ASTNode *then_br, ASTNode *else_br);
ASTNode *ast_create_while(ASTNode *cond, ASTNode *body);
ASTNode *ast_create_return(ASTNode *value);
ASTNode *ast_create_binary_op(char *op, ASTNode *left, ASTNode *right);
ASTNode *ast_create_assign(char *name, ASTNode *value);
ASTNode *ast_create_call(char *name, ASTNode **args, int arg_count);
ASTNode *ast_create_variable(char *name);
ASTNode *ast_create_number(double value);
void ast_add_statement(ASTNode *block, ASTNode *stmt);
void ast_add_function(ASTNode *program, ASTNode *func);
void ast_free(ASTNode *node);
// Add this line
ASTNode *ast_create_node(ASTNodeType type); 

ASTNode *ast_create_program();
ASTNode *ast_create_function(char *name, DataType ret_type, ASTNode *body);
// ... rest of the prototypes

#endif