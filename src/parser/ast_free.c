/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 22:27:11 by madlen            #+#    #+#             */
/*   Updated: 2025/09/24 22:31:17 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

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