#include "../../include/parser/parser.h"


// Grammar rules for a simple shell parser
subshell && || | cmd 
parse_line
    ↳ parse_logical   // handles && and ||
        ↳ parse_pipeline   // handles |
            ↳ parse_command   // a simple command
                ↳ parse_redirection   // attaches < > >> <<




t_ast_node **parse_logicale_operator(t_token **current_token)
{

	t_ast_node *node;
    t_ast_node *new_node;

	node = parse_pipeline(current_token);
	while(*current_token && ((*current_token)->value->ctrl_op_type == CTRL_OP_AND || (*current_token)->value->ctrl_op_type == CTRL_OP_OR))
	{
        new_node = malloc(sizeof(t_ast_node));
        if (!new_node)
            return (NULL);
        new_node->value = *current_token;
        new_node->left = node;
        current_token = (*current_token)->next;
        new_node->right = parse_pipeline(current_token);
        node = new_node;
	}
    return (node);
}

t_ast_node **parse_pipeline(t_token **current_token)
{
    t_ast_node *node;
    t_ast_node *new_node;

    node = parse_command(current_token);
    while(*current_token && (*current_token)->value->ctrl_op_type == CTRL_OP_PIPE)
    {
        new_node = malloc(sizeof(t_ast_node));
        if (!new_node)
            return (NULL);
        new_node->value = *current_token;
        new_node->left = node;
        current_token = (*current_token)->next;
        new_node->right = parse_command(current_token);
        node = new_node;
    }
    return (node);
}

t_ast_node **parse_command(t_token **current_token)
{   
    t_ast_node *node;
    if((*current_token)->value->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
        node = parse_subshell(current_token);
    else if((*current_token)->value->token_type == TOKEN_WORD)
        {
            node = malloc(sizeof(t_ast_node));
            if (!node)
                return (NULL);
            node->value = *current_token;
            current_token = (*current_token)->next;
            node->left = NULL;
            node->right = NULL;
        }
}

t_ast_node **parse_subshell(t_token **current_token)
{
    t_ast_node *node;
    t_ast_node *new_node;

    if (*current_token && (*current_token)->value->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
    {
        new_node = malloc(sizeof(t_ast_node));
        if (!new_node)
            return (NULL);
        new_node->value = *current_token;
        current_token = (*current_token)->next;
        new_node->left = parse_logical_operator(current_token);
        if (*current_token && (*current_token)->value->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
        {
            new_node->right = *current_token;
            current_token = (*current_token)->next;
            return (new_node);
        }
    }
    return (NULL);
}

t_ast_node **parse_pipeline(t_token **current_token)
{
    t_ast_node *node;
    t_ast_node *new_node;

    node = parse_command(current_token);
    while(*current_token && (*current_token)->value->ctrl_op_type == CTRL_OP_PIPE)
    {
        new_node = malloc(sizeof(t_ast_node));
        if (!new_node)
            return (NULL);
        new_node->value = *current_token;
        new_node->left = node;
        current_token = (*current_token)->next;
        new_node->right = parse_command(current_token);
        node = new_node;
    }
    return (node);
}

t_ast_node **parse_subshell(t_token **current_token)
{
    t_ast_node *node;
    t_ast_node *new_node;

    if (*current_token && (*current_token)->value->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
    {
        new_node = malloc(sizeof(t_ast_node));
        if (!new_node)
            return (NULL);
        new_node->value = *current_token;
        current_token = (*current_token)->next;
        new_node->left = parse_logical_operator(current_token);
        if (*current_token && (*current_token)->value->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
        {
            new_node->right = *current_token;
            current_token = (*current_token)->next;
            return (new_node);
        }
    }
    return (NULL);
}


t_ast_node **parse_redirection(t_token **current_token)
{

}




