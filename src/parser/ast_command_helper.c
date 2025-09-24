/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_command_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 22:20:50 by madlen            #+#    #+#             */
/*   Updated: 2025/09/24 22:34:24 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"


bool process_word_token(t_ast_node *node, t_command *cmd,  t_token *token, bool *has_command)
{
	if (!*has_command && !node->value)
	{
		node->value = token;
		*has_command = true;
	}
	if (!append_argument(cmd, token->value))
		return (false);
	return (true);
}

void parse_command_tokens(t_ast_node *node, t_command *cmd, t_list **current_token)
{
	bool has_command;
	t_token *token;

	has_command = false;
	while (*current_token)
	{
		token = (t_token *)(*current_token)->content;
		if (token->token_type == TYPE_WORD)
		{
			if (!process_word_token(node, cmd, token, &has_command))
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
}
t_ast_node *build_command_node(t_list **current_token)
{
	t_ast_node *node;

	node = init_command_node();
	if (!node)
		return (NULL);
	parse_command_tokens(node, node->command, current_token);
	return (node);
}

