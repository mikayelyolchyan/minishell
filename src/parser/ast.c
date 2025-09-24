#include "../../include/parser/parser.h"


// Grammar rules for a simple shell parser
/*subshell && || | cmd 
parse_line
    ↳ parse_logical   // handles && and ||
        ↳ parse_pipeline   // handles |
            ↳ parse_command   // a simple command
                ↳ parse_redirection   // attaches < > > >>

*/

/*typedef struct  s_redirection_list
{
    t_redirection_operator_type     type;
    char            *target;      
    struct s_redirection_list  *next;       
}   t_redirection_list;



typedef struct s_command_list
{
    char        **arguments;          
    struct s_redirection_list     *redirections;
}   t_command;

typedef struct s_ast_node
{
    //t_node_type type;
    
    t_token *token;
    t_command *command;  
    
    struct s_ast_node *left;
    struct s_ast_node *right;

}   t_ast_node;




t_ast_node *build_ast(t_list *token)
{
    return(ast_logical(&token));
}
t_ast_node *ast_logical(t_list **current_token);

t_ast_node *ast_logical(t_list **current_token)
{
    t_ast_node *left_child;
    t_ast_node *new_node;
    t_token *token;

    if(!current_token || !*current_token)
        return (NULL);
    left_child = ast_pipeline(current_token);
    token = (t_token *)(*current_token)->content;
    while(*current_token && (token->ctrl_op_type == CTRL_OP_AND || token->ctrl_op_type == CTRL_OP_OR))
    {
            new_node = malloc(sizeof(t_ast_node));
            if(!new_node)
                return(free_ast(),NULL);
            new_node->left = left_child;
            new_node->token = token;
            new_node->command = NULL;
            (*current_token) = (*current_token)->next;
            token = (t_token *)(*current_token)->content;
            new_node->right = ast_pipeline(current_token);
            left_child = new_node;
    }
    return(left_child);
}


t_ast_node *ast_pipeline(t_list **current_token)
{
     t_ast_node *left_child;
    t_ast_node *new_node;
    t_token *token;

    if(!current_token | !*current_token)
        return (NULL);
    left_child = ast_command(current_token);
    token = (t_token *)(*current_token)->content;
    while(*current_token && token->ctrl_op_type == CTRL_OP_PIPE)
    {
            token = (t_token *)(*current_token)->content;
            new_node = malloc(sizeof(t_ast_node));
            if(!new_node)
                return(NULL);
            new_node->left = left_child;
            new_node->token = token;
            new_node->command = NULL;
            (*current_token) = (*current_token)->next;
            token = (t_token *)(*current_token)->content;
            new_node->right = ast_command(current_token);
            left_child = new_node;
    }
    return(left_child);
}

t_ast_node *ast_subshell(t_list **current_token)
{
    t_ast_node *new_node;
    t_token *token;

    token = (t_token *)(*current_token)->content;
    if (*current_token && token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
    {
        while(*current_token && token->ctrl_op_type  != CTRL_OP_SUBSHELL_CLOSE)
        {
            new_node = malloc(sizeof(t_ast_node));
            if (!new_node)
                return (NULL);
            current_token = (*current_token)->next;
            new_node->left = ast_logical(current_token);
            current_token = (*current_token)->next;
            new_node->right = NULL;
            new_node->command = NULL;
            new_node->token = NULL;
        }
    }
    return (new_node);
}

t_ast_node *ast_command(t_list **current_token)
{
    t_ast_node *new_node;
    t_token *token;
    t_command *command;
    t_redirection_list redir;

    command = malloc(sizeof(t_command));
    if (!command)
        return (NULL);
    command->arguments = NULL;     
    command->redirections = NULL

    while(*current_token && (token->token_type == TYPE_WORD || token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN))
    {
        token = (t_token *)(*current_token)->content;
        if(token->ctrl_op_type == TYPE_WORD)
        {
         new_node = malloc(sizeof(t_ast_node));
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->token = NULL;
        commands->arguments = ft_strjoin(token->value);
        }
        else if(*current_token && token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
        {
            new_node = ast_subshell(current_token);
            *current_token = (*current_token)->next;
        }
    }
    return (new_node);
}

/* t_ast_node *parse_logicale_operator(t_list **current_token)
{

	t_ast_node *node;
    t_ast_node *new_node;
    t_list *current;
    t_token *tokens;

    tokens = (t_token *)(*current_token)->content;

	node = parse_pipeline(current_token);
	while(*current_token && (tokens->ctrl_op_type == CTRL_OP_AND || tokens->value->ctrl_op_type == CTRL_OP_OR))
	{
        new_node = malloc(sizeof(t_ast_node));
        if (!new_node)
            return (NULL);
        new_node->content = tokens;
        new_node->left = node;
        tokens = (t_token *)(*current_token)->next->content;
        new_node->right = parse_pipeline(tokens);
        node = new_node;
	}
    return (node);
}

t_ast_node *parse_pipeline(t_list **current_token)
{
    t_ast_node *node;
    t_ast_node *new_node;
    t_token *tokens;
    t_list *current;

    if (!current_token || !*current_token)
        return (NULL);

    tokens = (t_token *)(*current_token)->content;
    node = parse_command(current_token);
    while(*current_token && tokens->ctrl_op_type == CTRL_OP_PIPE)
    {
        new_node = malloc(sizeof(t_ast_node));
        if (!new_node)
            return (NULL);
        new_node->left = node;
        tokens  = tokens->next->content;
        new_node->right = parse_command(current_token);
        node = new_node;
    }
    return (node);
}

t_ast_node *parse_command(t_list **current_token)
{   
    t_ast_node *node;
    t_tokens *tokens;

    tokens = (t_token *)(*current_token)->content;
    if(tokens->content->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
        node = parse_subshell(tokens);//shouldnt I give the *current_token ? so it can advance ? 
    else if(tokens->content->token_type == TYPE_WORD)
        {
            node = malloc(sizeof(t_ast_node));
            if (!node)
                return (NULL);
            node->value = *current_token;
            tokens = (t_tokens *)(*current_token)->next->content;
            node->left = NULL;
            node->right = NULL;
        }
    else if (tokes->content->token_type == TYPE_REDIRECTION_OPERATOR)
        {


        }
    else
        return (NULL);
    return (node);
}


t_ast_node *parse_subshell(t_list **current_token)
{
    t_ast_node *new_node;
    t_token *tokes;

    tokens = (t_token *)(*current_token)->content;
    
    if ( tokens && tokens->content->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
    {
        new_node = malloc(sizeof(t_ast_node));
        if (!new_node)
            return (NULL);
        new_node->value = *current_token;
        *current_token = (*current_token)->next; 
        new_node->left = parse_logical_operator(current_token);  
        if (*current_token && ((t_token *)(*current_token)->content)->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
        {
            new_node->right = *current_token;
            *current_token = (*current_token)->next;  
            return (new_node);
        }
    }
    return (NULL);

}


/*t_ast_node *parse_logicale_operator(t_list **current_token)
{

	t_ast_node *node;
    t_ast_node *new_node;

	node = parse_pipeline(current_token);
	while(*current_token && (((t_token *)(*current_token)->content)->ctrl_op_type == CTRL_OP_AND || (*current_token)->value->ctrl_op_type == CTRL_OP_OR))
	{
        new_node = malloc(sizeof(t_ast_node));
        if (!new_node)
            return (NULL);
        new_node->content = *current_token;
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
    else if (((t_token *)(*current_token)->content)->token_type == TYPE_REDIRECTION_OPERATOR)
        {


        }
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
}*/




