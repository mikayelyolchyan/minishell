/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:00:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/22 16:33:43 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

static bool	check_subshell_open(t_token *next)
{
	if (next && (next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN || \
		next->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE))
		return (true);
	if (next && (next->token_type == TYPE_CONTROL_OPERATOR || \
		next->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE))
	{
		print_syntax_error(next->value);
		return (false);
	}
	return (true);
}

static bool	check_subshell_close(t_token *next)
{
	if (next && next->token_type == TYPE_WORD)
	{
		print_syntax_error(next->value);
		return (false);
	}
	return (true);
}

bool	check_subshell_syntax(t_list *current_token)
{
	t_token	*current;
	t_token	*next;

	current = (t_token *)current_token->content;
	next = (t_token *)current_token->next->content;
	if (current->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
	{
		if (!check_subshell_open(next))
			return (false);
		if (next && (next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN || \
			next->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE))
			return (check_subshell_syntax(current_token->next));
	}
	else if (current->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
		return (check_subshell_close(next));
	return (true);
}
