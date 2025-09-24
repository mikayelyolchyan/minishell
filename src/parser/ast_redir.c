/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 22:48:02 by madlen            #+#    #+#             */
/*   Updated: 2025/09/24 23:03:48 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

bool	handle_redirection(t_command *cmd, t_list **current_token)
{
	t_token	*token;
	t_redir	*new_redir;

	if (!current_token || !*current_token)
		return (false);
	token = (t_token *)(*current_token)->content;
	if (!is_redir(token))
		return (false);
	new_redir = init_redirection(token);
	if (!new_redir)
		return (false);
	if (!assign_redirection_filename(new_redir, current_token))
		return (free(new_redir), false);
	insert_redirection(cmd, new_redir);
	*current_token = (*current_token)->next;
	return (true);
}

t_redir	*init_redirection(t_token *token)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->redir_type = token->redir_op_type;
	new_redir->filename = NULL;
	new_redir->next = NULL;
	return (new_redir);
}

bool	assign_redirection_filename(t_redir *new_redir, t_list **current_token)
{
	t_token	*token;

	*current_token = (*current_token)->next;
	if (!*current_token)
		return (false);
	token = (t_token *)(*current_token)->content;
	if (token->token_type != TYPE_WORD)
		return (false);
	new_redir->filename = ft_strdup(token->value);
	if (!new_redir->filename)
		return (false);
	return (true);
}

void	insert_redirection(t_command *cmd, t_redir *new_redir)
{
	t_redir	*tmp;

	if (!cmd->redir)
	{
		cmd->redir = new_redir;
		return ;
	}
	tmp = cmd->redir;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_redir;
}
