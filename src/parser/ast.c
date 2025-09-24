/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 21:44:51 by madlen            #+#    #+#             */
/*   Updated: 2025/09/24 23:04:18 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

t_ast_node	*ast_command(t_list **current_token)
{
	t_token	*token;

	if (!current_token || !*current_token)
		return (NULL);
	token = (t_token *)(*current_token)->content;
	if (token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
		return (ast_subshell(current_token));
	if (token->token_type != TYPE_WORD && !is_redir(token))
		return (NULL);
	return (build_command_node(current_token));
}

t_ast_node	*ast_pipeline(t_list **current_token)
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
			return (free_ast(left_child), NULL);
		new_node->left = left_child;
		*current_token = (*current_token)->next;
		new_node->right = ast_command(current_token);
		left_child = new_node;
		if (*current_token)
			token = (t_token *)(*current_token)->content;
	}
	return (left_child);
}

t_ast_node	*ast_logical(t_list **current_token)
{
	t_ast_node	*left_child;
	t_ast_node	*new_node;
	t_token		*token;

	if (!current_token || !*current_token)
		return (NULL);
	left_child = ast_pipeline(current_token);
	if (!*current_token)
		return (left_child);
	token = (t_token *)(*current_token)->content;
	while (*current_token && (token->ctrl_op_type == CTRL_OP_AND
			|| token->ctrl_op_type == CTRL_OP_OR))
	{
		new_node = create_ast_node(token);
		if (!new_node)
			return (free_ast(left_child), NULL);
		new_node->left = left_child;
		*current_token = (*current_token)->next;
		new_node->right = ast_pipeline(current_token);
		left_child = new_node;
		if (*current_token)
			token = (t_token *)(*current_token)->content;
	}
	return (left_child);
}

t_ast_node	*build_ast(t_list *token)
{
	return (ast_logical(&token));
}

t_ast_node	*ast_subshell(t_list **current_token)
{
	t_ast_node	*new_node;
	t_token		*token;

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
	return (NULL);
}
