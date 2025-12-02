/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 22:50:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/02 22:50:00 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expansion/expansion.h"
#include "../../include/parser/parser.h"
#include <stdlib.h>

char	*join_and_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 || !s2)
	{
		if (s1)
			free(s1);
		if (s2)
			free(s2);
		return (NULL);
	}
	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

static void	expand_arguments(char **args, t_shell *shell)
{
	int		i;
	char	*expanded;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		expanded = expand_variables(args[i], shell);
		if (expanded)
		{
			free(args[i]);
			args[i] = expanded;
		}
		i++;
	}
}

static void	expand_redirections(t_redir *redir, t_shell *shell)
{
	char	*expanded;

	while (redir)
	{
		if (redir->filename && redir->redir_type != REDIR_OP_HERE_DOC)
		{
			expanded = expand_variables(redir->filename, shell);
			if (expanded)
			{
				free(redir->filename);
				redir->filename = expanded;
			}
		}
		redir = redir->next;
	}
}

void	expand_ast_node(t_ast_node *node, t_shell *shell)
{
	if (!node)
		return ;
	if (node->left)
		expand_ast_node(node->left, shell);
	if (node->right)
		expand_ast_node(node->right, shell);
	if (node->command)
	{
		expand_arguments(node->command->argument, shell);
		expand_redirections(node->command->redir, shell);
	}
}
