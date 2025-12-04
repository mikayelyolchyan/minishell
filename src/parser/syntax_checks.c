/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 20:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 20:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

bool	chek_here_doc_count(t_list *tokens)
{
	t_token	*current_token;
	t_token	*last_token;
	int		here_doc_count;

	here_doc_count = 0;
	while (tokens && tokens->next)
	{
		current_token = (t_token *)tokens->content;
		if (current_token->redir_op_type == REDIR_OP_HERE_DOC)
			here_doc_count++;
		tokens = tokens->next;
	}
	if (tokens)
	{
		last_token = (t_token *)tokens->content;
		if (last_token->redir_op_type == REDIR_OP_HERE_DOC)
			here_doc_count++;
	}
	if (here_doc_count >= 16)
		return (false);
	return (true);
}

bool	check_operator_combinations_for_redirection(t_list *tokens)
{
	t_list	*current;
	t_token	*token1;
	t_token	*token2;

	current = tokens;
	while (current && current->next)
	{
		token1 = (t_token *)current->content;
		token2 = (t_token *)current->next->content;
		if (token1->token_type == TYPE_REDIRECTION_OPERATOR
			&& token2->token_type == TYPE_REDIRECTION_OPERATOR)
		{
			print_syntax_error(token2->value);
			return (false);
		}
		current = current->next;
	}
	return (true);
}

bool	check_subshell_syntax(t_list *current_token)
{
	t_token	*current;
	t_token	*next;

	current = (t_token *)current_token->content;
	if (!current_token->next)
		return (print_syntax_error("newline"), false);
	next = (t_token *)current_token->next->content;
	if (current->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
	{
		if (next && (next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN
				|| next->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE))
			return (check_subshell_syntax(current_token->next));
		if (next && (next->token_type == TYPE_CONTROL_OPERATOR
				|| next->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE))
			return (print_syntax_error(next->value), false);
	}
	else if (current->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
	{
		if (next && next->token_type == TYPE_WORD)
			return (print_syntax_error(next->value), false);
		if (next && next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
			return (print_syntax_error(next->value), false);
	}
	return (true);
}

bool	chek_ctrl_operator_syntax(t_list *current_token)
{
	t_token	*current;
	t_token	*next;

	current = (t_token *)current_token->content;
	next = (t_token *)current_token->next->content;
	if (!next)
		return (print_syntax_error("newline"), false);
	else if (current->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
		return (check_subshell_syntax(current_token));
	else if (next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN
		|| next->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
		return (check_subshell_syntax(current_token));
	else if (next && next->token_type == TYPE_CONTROL_OPERATOR)
		return (print_syntax_error(next->value), false);
	return (true);
}

bool	process_token_list(t_list *tokens, bool (*validate_fn)(t_list *))
{
	t_list	*current_list;

	current_list = tokens;
	while (current_list && current_list->next)
	{
		if (!check_token_syntax(current_list)
			|| !chek_closed_quotes(current_list))
		{
			if (chek_closed_quotes(current_list))
				return (false);
			return (print_syntax_error("newline"), false);
		}
		current_list = current_list->next;
	}
	return (validate_fn(current_list));
}
