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
