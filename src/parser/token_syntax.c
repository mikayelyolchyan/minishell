/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:00:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/23 19:01:07 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

static bool	check_redirection_syntax(t_list *current_token)
{
	t_token	*current;
	t_token	*next;

	current = (t_token *)current_token->content;
	if (!current_token->next)
	{
		print_syntax_error("newline");
		return (false);
	}
	next = (t_token *)current_token->next->content;
	if (!next || next->token_type != TYPE_WORD)
	{
		if (next)
			print_syntax_error(next->value);
		else
			print_syntax_error("newline");
		return (false);
	}
	return (true);
}

static bool	check_word_syntax(t_list *current_token)
{
	t_token	*current;
	t_token	*next;

	current = (t_token *)current_token->content;
	if (!current_token->next)
		return (true);
	next = (t_token *)current_token->next->content;
	if (is_dollar_before_subshell(current, current_token))
		return (true);
	if (next && next->token_type == TYPE_CONTROL_OPERATOR && \
		next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
	{
		print_syntax_error(next->value);
		return (false);
	}
	return (true);
}

bool	check_token_syntax(t_list *current_token)
{
	t_token	*current;

	if (!current_token || !current_token->content)
		return (true);
	current = (t_token *)current_token->content;
	if (current->token_type == TYPE_CONTROL_OPERATOR)
		return (check_ctrl_operator_syntax(current_token));
	else if (current->token_type == TYPE_REDIRECTION_OPERATOR)
		return (check_redirection_syntax(current_token));
	else if (current->token_type == TYPE_WORD)
		return (check_word_syntax(current_token));
	return (true);
}
