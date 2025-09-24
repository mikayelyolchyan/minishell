/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:58 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/24 18:31:29 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"


bool is_redir(t_token *token)
{
    if (!token)
        return false;
    return (token->redir_op_type != REDIR_OP_NONE);
}
void free_string_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

void free_redir_list(t_redir *redir)
{
    t_redir *tmp;
    if(!redir)
        return;
    while (redir)
    {
        tmp = redir->next;
        free(redir->filename);
        free(redir);
        redir = tmp;
    }
}

void free_command(t_command *cmd)
{
    if (!cmd) 
        return;
    free_string_array(cmd->argument);
    free_redir_list(cmd->redir);
    free(cmd);
}

void free_ast(t_ast_node *node)
{
    if (!node)
        return;
    free_ast(node->left);
    free_ast(node->right);
    if (node->command)
        free_command(node->command);
    free(node);
}


 t_ast_node *create_ast_node(t_token *token)
{
    t_ast_node *node;

    node = malloc(sizeof(t_ast_node));
    if (!node)
        return (NULL);
    node->value = token;
    node->left = NULL;
    node->right = NULL;
	node->command = NULL;
    return (node);
}
t_ast_node *ast_pipeline(t_list **current_token)
{
	t_ast_node	*left_child;
	t_ast_node	*new_node;
	t_token		*token;

	if (!current_token || !*current_token)
		return (NULL);
	left_child = ast_command(current_token);
	if (!*current_token)
		return (left_child);
	token = (t_token *)(*current_token)->content;
	while (*current_token && token->ctrl_op_type == CTRL_OP_PIPE)
	{
		new_node = create_ast_node(token);
		if (!new_node)
		{
			free_ast(left_child);
			return (NULL);
		}
		new_node->left = left_child;
		*current_token = (*current_token)->next;
		new_node->right = ast_command(current_token);
		left_child = new_node;
		if (*current_token)
			token = (t_token *)(*current_token)->content;
	}
	return (left_child);
}

t_ast_node *ast_logical(t_list **current_token)
{
	t_ast_node *left_child;
	t_ast_node *new_node;
	t_token *token;

	if (!current_token || !*current_token)
		return (NULL);
	left_child = ast_pipeline(current_token);
	if (!*current_token)
		return (left_child);
	token = (t_token *)(*current_token)->content;
	while (*current_token && (token->ctrl_op_type == CTRL_OP_AND || 
							  token->ctrl_op_type == CTRL_OP_OR))
	{
		new_node = create_ast_node(token);
		if (!new_node)
		{
			free_ast(left_child);
			return (NULL);
		}
		new_node->left = left_child;
		*current_token = (*current_token)->next;
		new_node->right = ast_pipeline(current_token);
		left_child = new_node;
		if (*current_token)
			token = (t_token *)(*current_token)->content;
	}
	return (left_child);
}

t_ast_node *build_ast(t_list *token)
{
    return (ast_logical(&token));
}

t_ast_node *ast_subshell(t_list **current_token)
{
    t_ast_node *new_node;
    t_token *token;

    if (!current_token || !*current_token)
        return (NULL);
    token = (t_token *)(*current_token)->content;
    if (token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
    {
        new_node = create_ast_node(token);
        if (!new_node)
            return (NULL);
        *current_token = (*current_token)->next;
        new_node->left = ast_logical(current_token);
        if (*current_token && 
            ((t_token *)(*current_token)->content)->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
            *current_token = (*current_token)->next;
        return (new_node);
    }
    return (NULL); // ???
}



bool append_argument(t_command *cmd, const char *arg)
{
    int     count;
    char    **new_args;
    int     i;

    count = 0;
    if (cmd->argument)
    {
        while (cmd->argument[count])
            count++;
    }

    new_args = malloc(sizeof(char *) * (count + 2));
    if (!new_args)
        return (false);

    i = 0;
    while (i < count)
    {
        new_args[i] = cmd->argument[i]; // reuse old strings
        i++;
    }

    new_args[count] = ft_strdup(arg);
    if (!new_args[count])
    {
        free(new_args);
        return (false);
    }
    new_args[count + 1] = NULL;

    free(cmd->argument); // free only the old array
    cmd->argument = new_args;

    return (true);
}


/*bool handle_redirection(t_command *cmd, t_list **current_token)
{
    t_token *token;
    t_redir *new_redir;
    t_redir *tmp;
    
    token =  (t_token *)(*current_token)->content;
    if (!is_redir(token))
        return false; 
    new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
        return false;
    new_redir->redir_type = token->redir_op_type;
    new_redir->filename = NULL;
    new_redir->next = NULL;
    *current_token = (*current_token)->next;
    if (!*current_token)
        return (free(new_redir),false);
    token = (t_token *)(*current_token)->content;
    if (token->token_type != TYPE_WORD)
        return (free(new_redir),false);
    new_redir->filename = ft_strdup(token->value);
    if (!new_redir->filename)
        return (free(new_redir),false);
    if (!cmd->redir)
        cmd->redir = new_redir;
    else
    {
        tmp = cmd->redir;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_redir;
    }
    *current_token = (*current_token)->next;
    return true;
}

t_ast_node *ast_command(t_list **current_token)
{
    t_ast_node *new_node;
    t_token    *token;
    t_command  *cmd;

    if (!current_token || !*current_token)
        return NULL;

    token = (t_token *)(*current_token)->content;
    if (token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
        return ast_subshell(current_token);
    if (token->token_type != TYPE_WORD)
        return (NULL);
    new_node = create_ast_node(token);
    if (!new_node)
        return NULL;
    cmd = ft_calloc(1, sizeof(t_command));
    if (!cmd)
        return (free(new_node), NULL);
    new_node->command = cmd;
    while (*current_token)
    {
        token = (t_token *)(*current_token)->content;
        if (token->token_type == TYPE_WORD)
        {
            if (!append_argument(cmd, token->value))
                break;
            *current_token = (*current_token)->next;
        }
        else if (is_redir(token))
        {
            if (!handle_redirection(cmd, current_token))
                break;
        }
        else
            break;
    }

    return new_node;
}*/


t_ast_node *ast_command(t_list **current_token)
{
    t_ast_node *new_node;
    t_token    *token;
    t_command  *cmd;
    bool       has_command = false;

    if (!current_token || !*current_token)
        return NULL;

    token = (t_token *)(*current_token)->content;
    
    // Handle subshells
    if (token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
        return ast_subshell(current_token);
    
    // Check if we start with a redirection or a word
    if (token->token_type != TYPE_WORD && !is_redir(token))
        return NULL;

    // Create a dummy node for commands that start with redirections
    new_node = create_ast_node(NULL);  // NULL token for redirection-only commands
    if (!new_node)
        return NULL;
        
    cmd = ft_calloc(1, sizeof(t_command));
    if (!cmd)
        return (free(new_node), NULL);
    new_node->command = cmd;

    // Parse the command - handle redirections and words in any order
    while (*current_token)
    {
        token = (t_token *)(*current_token)->content;
        
        if (token->token_type == TYPE_WORD)
        {
            // If this is the first word and we haven't set a command token yet
            if (!has_command && !new_node->value)
            {
                new_node->value = token;  // Set this as the command token
                has_command = true;
            }
            
            if (!append_argument(cmd, token->value))
                break;
            *current_token = (*current_token)->next;
        }
        else if (is_redir(token))
        {
            if (!handle_redirection(cmd, current_token))
                break;
        }
        else
            break; // Stop at operators
    }

    // If we never found a command word, this might be a pure redirection
    // which is valid in some shells but might need special handling
    return new_node;
}

// Also need to update handle_redirection to be more robust
bool handle_redirection(t_command *cmd, t_list **current_token)
{
    t_token *token;
    t_redir *new_redir;
    t_redir *tmp;
    
    if (!current_token || !*current_token)
        return false;
        
    token = (t_token *)(*current_token)->content;
    if (!is_redir(token))
        return false; 
        
    new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
        return false;
        
    new_redir->redir_type = token->redir_op_type;
    new_redir->filename = NULL;
    new_redir->next = NULL;
    
    *current_token = (*current_token)->next;
    
    // Check if we have a filename after the redirection
    if (!*current_token)
    {
        free(new_redir);
        return false;
    }
    
    token = (t_token *)(*current_token)->content;
    if (token->token_type != TYPE_WORD)
    {
        free(new_redir);
        return false;
    }
    
    new_redir->filename = ft_strdup(token->value);
    if (!new_redir->filename)
    {
        free(new_redir);
        return false;
    }
    
    // Add to the redirection list
    if (!cmd->redir)
        cmd->redir = new_redir;
    else
    {
        tmp = cmd->redir;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_redir;
    }
    
    *current_token = (*current_token)->next;
    return true;
}

// Updated is_redir function to handle your token structure



/*void print_ast_command(t_ast_node *node)
{
    if (!node || !node->command)
        return;

    printf("COMMAND NODE:\n");

    // Print arguments
    if (node->command->argument)
    {
        printf("  Arguments: ");
        for (int i = 0; node->command->argument[i]; i++)
            printf("[%s] ", node->command->argument[i]);
        printf("\n");
    }

    // Print redirections
    t_redir *r = node->command->redir;
    while (r)
    {
        printf("  Redirection: type=%d, target=%s\n",
               r->redir_type, r->filename);
        r = r->next;
    }
}*/

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


 /*t_ast_node *ast_command(t_list **current_token)
{
    t_ast_node	*new_node;
    t_token		*token;

    if (!current_token || !*current_token)
        return (NULL);
    token = (t_token *)(*current_token)->content;
    // Обрабатываем подоболочку
    if (token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
        return (ast_subshell(current_token));
    // Обрабатываем команды и слова
    if (token->token_type == TYPE_WORD)
    {
        new_node = create_ast_node(token);
        if (!new_node)
            return (NULL);
        *current_token = (*current_token)->next;
        // Добавляем аргументы как левые дочерние узлы
        while (*current_token)
        {
            token = (t_token *)(*current_token)->content;
            if (token->token_type == TYPE_WORD)
            {
                if (!new_node->left)
                    new_node->left = create_ast_node(token);
                *current_token = (*current_token)->next;
            }
            else
                break;
        }
        return (new_node);
    }
    return (NULL);
}*/


/*t_ast_node *ast_command(t_list **current_token)
{
    t_ast_node   *new_node;
    t_token      *token;
    t_command    *cmd;
    char         **args;
    int           argc;
    t_redir      *redir_head = NULL;
    t_redir      *redir_tail = NULL;

    
    if (!current_token || !*current_token)
        return (NULL);
    token = (t_token *)(*current_token)->content;

    // Handle subshell
    if (token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
        return (ast_subshell(current_token));

    // Must start with a word (command)
    if (token->token_type != TYPE_WORD)
        return (NULL);

    // Create AST node
    new_node = create_ast_node(token);
    if (!new_node)
        return (NULL);

    // Create command struct
    cmd = malloc(sizeof(t_command));
    if (!cmd)
    {
        free(new_node);
        return (NULL);
    }
    cmd->argument = NULL;
    cmd->redir = NULL;

    // Collect arguments (dynamic array)
    argc = 0;
    args = NULL;

    while (*current_token)
    {
        token = (t_token *)(*current_token)->content;

        if (token->token_type == TYPE_WORD)
        {
            // Append argument
            char **tmp = realloc(args, sizeof(char *) * (argc + 2));
            if (!tmp)
                break;
            args = tmp;
            args[argc] = strdup(token->value); // copy string
            args[argc + 1] = NULL;
            argc++;
            *current_token = (*current_token)->next;
        }
        else if (is_redir(token)) // redirection handling
        {
            t_redir *new_redir = malloc(sizeof(t_redir));
            if (!new_redir)
                break;
            new_redir->redir_type = token->redir_op_type;

            *current_token = (*current_token)->next;
            if (!*current_token) // redirection requires target
                break;
            token = (t_token *)(*current_token)->content;
            if (token->token_type != TYPE_WORD)
                break;
            new_redir->filename = strdup(token->value);
            new_redir->next = NULL;

            if (!redir_head)
                redir_head = new_redir;
            else
                redir_tail->next = new_redir;
            redir_tail = new_redir;

            *current_token = (*current_token)->next;
        }
        else
            break;
    }

    cmd->argument = args;
    cmd->redir = redir_head;
    new_node->command = cmd;
    return (new_node);
}*/


/*bool append_argument(t_command *cmd, const char *arg)
{
    int count = 0;
    char **new_args;
    if (cmd->argument)
        while (cmd->argument[count])
            count++;
    new_args = realloc(cmd->argument, sizeof(char *) * (count + 2));
    if (!new_args)
        return false;
    new_args[count] = ft_strdup(arg);
    if (!new_args[count])
        return false;
    new_args[count + 1] = NULL;
    cmd->argument = new_args;
    return true;
}*/




