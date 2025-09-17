#include "../../include/parser/parser.h"


// Grammar rules for a simple shell parser
subshell && || | cmd 
parse_line
    ↳ parse_logical   // handles && and ||
        ↳ parse_pipeline   // handles |
            ↳ parse_command   // a simple command
                ↳ parse_redirection   // attaches < > >> <<

typedef struct s_redir
{
    t_redir_type     type;
    char            *target;      
    struct s_redir  *next;       
}   t_redir;

typedef struct s_command
{
    char        **args;          // Command and its arguments
    t_redir     *redirections;   // Linked list of redirections
}   t_command;




t_ast_node *parse_logicale_operator(t_list **current_token)
{

	t_ast_node *node;
    t_ast_node *new_node;

	node = parse_pipeline(current_token);
	while(*current_token && (((t_token *)(*current_token)->content)->ctrl_op_type == CTRL_OP_AND || (*current_token)->value->ctrl_op_type == CTRL_OP_OR))
	{
        new_node = malloc(sizeof(t_ast_node));
        if (!new_node)
            return (NULL);
        new_node->value = *current_token;
        new_node->left = node;
        *current_token = (*current_token)->next;
        new_node->right = parse_pipeline(current_token);
        node = new_node;
	}
    return (node);
}

t_ast_node *parse_pipeline(t_list **current_token)
{
    t_ast_node *node;
    t_ast_node *new_node;

    node = parse_command(current_token);
    while(*current_token && ((t_token *)(*current_token)->content)->ctrl_op_type == CTRL_OP_PIPE)
    {
        new_node = malloc(sizeof(t_ast_node));
        if (!new_node)
            return (NULL);
        new_node->value = *current_token;
        new_node->left = node;
        *current_token = (*current_token)->next;
        new_node->right = parse_command(current_token);
        node = new_node;
    }
    return (node);
}

t_ast_node *parse_command(t_list **current_token)
{   
    t_ast_node *node;
    if(((t_token *)(*current_token)->content)->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
        node = parse_subshell(current_token);
    else if(((t_token *)(*current_token)->content)->token_type == TYPE_WORD)
        {
            node = malloc(sizeof(t_ast_node));
            if (!node)
                return (NULL);
            node->value = *current_token;
            *current_token = (*current_token)->next;
            node->left = NULL;
            node->right = NULL;
        }
    else if ((*current_token)->content->token_type == TYPE_REDIRECTION_OPERATOR)
        node = parse_redirection(current_token);
    else
        return (NULL);
    return (node);
}


t_ast_node *parse_subshell(t_list **current_token)
{
    t_ast_node *new_node;
    
    if (*current_token &&   ((t_token *)(*current_token)->content)->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
    {
        new_node = malloc(sizeof(t_ast_node));
        if (!new_node)
            return (NULL);
        new_node->value = *current_token;
        *current_token = (*current_token)->next;  // Fixed: Correct pointer assignment
        new_node->left = parse_logical_operator(current_token);  // Fixed: Function name
        if (*current_token && ((t_token *)(*current_token)->content)->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
        {
            new_node->right = *current_token;
            *current_token = (*current_token)->next;  // Fixed: Correct pointer assignment
            return (new_node);
        }
    }
    return (NULL);
}




t_ast_node **parse_redirection(t_token **current_token)
{

}




