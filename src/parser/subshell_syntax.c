/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:00:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/23 18:56:52 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"
#include "../../lib/libft/libft.h"

static bool	is_dollar_subshell_pattern(t_token *current, t_token *next)
{
	return (current->token_type == TYPE_WORD && \
			ft_strncmp(current->value, "$", 1) == 0 && \
			next && next->token_type == TYPE_CONTROL_OPERATOR && \
			next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN);
}

static bool	check_subshell_open(t_token *next)
{
	if (!next)
	{
		print_syntax_error("newline");
		return (false);
	}
	if (next->token_type == TYPE_CONTROL_OPERATOR && \
		next->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
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

static bool	handle_subshell_open(t_list *current_token)
{
	t_token	*current;
	t_token	*next;

	current = (t_token *)current_token->content;
	if (!current_token->next)
		next = NULL;
	else
		next = (t_token *)current_token->next->content;
	if (!check_subshell_open(next))
		return (false);
	if (next && next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
		return (check_subshell_syntax(current_token->next));
	return (true);
}

bool	check_subshell_syntax(t_list *current_token)
{
	t_token	*current;
	t_token	*next;

	if (!current_token || !current_token->content)
		return (true);
	current = (t_token *)current_token->content;
	if (!current_token->next)
		next = NULL;
	else
		next = (t_token *)current_token->next->content;
	if (is_dollar_subshell_pattern(current, next))
		return (true);
	if (current->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
		return (handle_subshell_open(current_token));
	else if (current->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
		return (check_subshell_close(next));
	return (true);
}
