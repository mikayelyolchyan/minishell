/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_operator_syntax.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:00:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/22 16:30:43 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

static bool	check_next_is_subshell(t_token *next)
{
	return (next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN || \
			next->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE);
}

static bool	check_next_is_control_op(t_token *next)
{
	if (next && next->token_type == TYPE_CONTROL_OPERATOR)
	{
		print_syntax_error(next->value);
		return (false);
	}
	return (true);
}

bool	check_ctrl_operator_syntax(t_list *current_token)
{
	t_token	*current;
	t_token	*next;

	current = (t_token *)current_token->content;
	next = (t_token *)current_token->next->content;
	if (!next)
	{
		print_syntax_error("newline");
		return (false);
	}
	if (current->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
		return (check_subshell_syntax(current_token));
	if (check_next_is_subshell(next))
		return (check_subshell_syntax(current_token));
	return (check_next_is_control_op(next));
}
