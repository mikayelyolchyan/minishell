/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 22:50:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/03 20:25:24 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expansion/expansion.h"
#include "../../include/parser/parser.h"

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

static char	*remove_quotes_from_string(char *str)
{
	char	*result;
	int		i;
	int		j;
	char	quote;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			quote = str[i++];
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

void	expand_arguments(char **args, t_shell *shell)
{
	int		i;
	char	*expanded;
	char	*no_quotes;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		expanded = expand_variables(args[i], shell);
		if (expanded)
		{
			no_quotes = remove_quotes_from_string(expanded);
			free(args[i]);
			free(expanded);
			args[i] = no_quotes;
		}
		i++;
	}
}

void	expand_redirections(t_redir *redir, t_shell *shell)
{
	char	*expanded;
	char	*no_quotes;

	while (redir)
	{
		if (redir->filename && redir->redir_type != REDIR_OP_HERE_DOC)
		{
			expanded = expand_variables(redir->filename, shell);
			if (expanded)
			{
				no_quotes = remove_quotes_from_string(expanded);
				free(redir->filename);
				free(expanded);
				redir->filename = no_quotes;
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
