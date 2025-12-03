/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyze.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:58 by miyolchy          #+#    #+#             */
/*   Updated: 2025/11/11 20:58:37 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

static bool	check_redirection_syntax(t_token *current, t_token *next)
{
	(void)current;
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

bool	check_token_syntax(t_list *current_token)
{
	t_token	*current;
	t_token	*next;

	current = (t_token *)current_token->content;
	next = (t_token *)current_token->next->content;
	if (current->token_type == TYPE_CONTROL_OPERATOR)
		return (chek_ctrl_operator_syntax(current_token));
	else if (current->token_type == TYPE_REDIRECTION_OPERATOR)
		return (check_redirection_syntax(current, next));
	else if (current->token_type == TYPE_WORD)
	{
		if (next && next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
			return (print_syntax_error(next->value), false);
	}
	return (true);
}

static bool	check_initial_token(t_token *token)
{
	if (token->token_type == TYPE_CONTROL_OPERATOR
		&& token->ctrl_op_type != CTRL_OP_SUBSHELL_OPEN)
	{
		print_syntax_error(token->value);
		return (false);
	}
	return (true);
}

static bool	validate_final_token(t_list *current_list)
{
	t_token	*current_token;

	current_token = (t_token *)current_list->content;
	if (current_token->token_type == TYPE_REDIRECTION_OPERATOR)
		return (print_syntax_error("newline"), false);
	if (current_token->token_type == TYPE_CONTROL_OPERATOR
		&& current_token->ctrl_op_type != CTRL_OP_SUBSHELL_CLOSE)
		return (print_syntax_error("newline"), false);
	if (current_token->token_type == TYPE_WORD
		&& !chek_closed_quotes(current_list))
		return (print_syntax_error("newline"), false);
	return (true);
}

bool	syntax_analyze(t_list *tokens)
{
	t_token	*current_token;

	if (!tokens)
		return (true);
	if (!chek_here_doc_count(tokens))
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
		return (false);
	}
	if (!check_subshell_balance(tokens)
		|| !check_operator_combinations_for_redirection(tokens))
		return (false);
	current_token = (t_token *)tokens->content;
	if (!check_initial_token(current_token))
		return (false);
	return (process_token_list(tokens, validate_final_token));
}
