/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:58 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/24 23:02:05 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

bool	is_redir(t_token *token)
{
	if (!token)
		return (false);
	return (token->redir_op_type != REDIR_OP_NONE);
}

t_ast_node	*create_ast_node(t_token *token)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->value = token;
	node->left = NULL;
	node->right = NULL;
	node->command = NULL;
	return (node);
}

bool	append_argument(t_command *cmd, const char *arg)
{
	int		count;
	char	**new_args;
	int		i;

	count = 0;
	if (cmd->argument)
	{
		while (cmd->argument[count])
			count++;
	}
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (false);
	i = -1;
	while (++i < count)
		new_args[i] = cmd->argument[i];
	new_args[count] = ft_strdup(arg);
	if (!new_args[count])
		return (free(new_args), false);
	new_args[count + 1] = NULL;
	free(cmd->argument);
	cmd->argument = new_args;
	return (true);
}

t_ast_node	*init_command_node(void)
{
	t_ast_node	*new_node;
	t_command	*cmd;

	new_node = create_ast_node(NULL);
	if (!new_node)
		return (NULL);
	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
	{
		free(new_node);
		return (NULL);
	}
	new_node->command = cmd;
	return (new_node);
}
/*bool append_argument(t_command *cmd, const char *arg)
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
		new_args[i] = cmd->argument[i]; 
		i++;
	}
	new_args[count] = ft_strdup(arg);
	if (!new_args[count])
		return ( free(new_args),false);
	new_args[count + 1] = NULL;
	free(cmd->argument);
	cmd->argument = new_args;
	return (true);
}*/
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

/*t_ast_node *ast_command(t_list **current_token)
{
	t_ast_node *new_node;
	t_token    *token;
	t_command  *cmd;
	bool       has_command = false;

	if (!current_token || !*current_token)
		return NULL;

	token = (t_token *)(*current_token)->content;
	if (token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
		return ast_subshell(current_token);
	if (token->token_type != TYPE_WORD && !is_redir(token))
		return NULL;
	new_node = create_ast_node(NULL);
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
			if (!has_command && !new_node->value)
			{
				new_node->value = token;
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
			break; 
	}
	return new_node;
}*/

/*bool handle_redirection(t_command *cmd, t_list **current_token)
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
	if (!*current_token)
		return (free(new_redir) ,false);
	token = (t_token *)(*current_token)->content;
	if (token->token_type != TYPE_WORD)
		return (free(new_redir),false);
	new_redir->filename = ft_strdup(token->value);
	if (!new_redir->filename)
		return (free(new_redir), false);
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
}*/

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
				args[argc] = ft_strdup(token->value);
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
				new_redir->filename = ft_strdup(token->value);
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