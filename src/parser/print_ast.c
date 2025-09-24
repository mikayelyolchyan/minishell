
#include "../../include/parser/parser.h"

void print_ast_recursive(t_ast_node *node, int depth)
{
	if (!node)
		return;

	// Print indentation for tree structure
	for (int i = 0; i < depth; i++)
		printf("  ");

	// Check if this is an operator node
	if (node->value)
	{
		t_token *token = node->value;
		
		// Handle control operators (&&, ||, |)
		if (token->ctrl_op_type == CTRL_OP_AND)
			printf("OPERATOR: &&\n");
		else if (token->ctrl_op_type == CTRL_OP_OR)
			printf("OPERATOR: ||\n");
		else if (token->ctrl_op_type == CTRL_OP_PIPE)
			printf("OPERATOR: |\n");
		else if (token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
			printf("SUBSHELL: (\n");
		else if (token->token_type == TYPE_WORD)
			printf("COMMAND: %s\n", token->value);
	}

	// If this node has a command, print its details
	if (node->command)
	{
		for (int i = 0; i < depth; i++)
			printf("  ");
		printf("├─ Arguments: ");
		
		if (node->command->argument)
		{
			for (int i = 0; node->command->argument[i]; i++)
				printf("[%s] ", node->command->argument[i]);
		}
		printf("\n");

		// Print redirections if any
		t_redir *r = node->command->redir;
		while (r)
		{
			for (int i = 0; i < depth; i++)
				printf("  ");
			printf("├─ Redirection: type=%d, target=%s\n",
				   r->redir_type, r->filename);
			r = r->next;
		}
	}

	// Recursively print children
	if (node->left)
	{
		for (int i = 0; i < depth; i++)
			printf("  ");
		printf("├─ LEFT:\n");
		print_ast_recursive(node->left, depth + 1);
	}
	
	if (node->right)
	{
		for (int i = 0; i < depth; i++)
			printf("  ");
		printf("└─ RIGHT:\n");
		print_ast_recursive(node->right, depth + 1);
	}
}

void print_ast(t_ast_node *ast)
{
	if (!ast)
	{
		printf("Empty AST\n");
		return;
	}
	
	printf("=== AST Structure ===\n");
	print_ast_recursive(ast, 0);
	printf("====================\n");
}