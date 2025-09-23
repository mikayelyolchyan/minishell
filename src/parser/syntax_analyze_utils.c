/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyze_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:36:28 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/23 18:58:33 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

void	print_syntax_error(char *token_value)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	ft_putstr_fd(token_value, 2);
	ft_putstr_fd("'\n", 2);
}

bool	is_dollar_before_subshell(t_token *current, t_list *tokens)
{
	t_token	*next;

	if (!tokens->next)
		return (false);
	next = (t_token *)tokens->next->content;
	return (current->token_type == TYPE_WORD && \
			ft_strncmp(current->value, "$", ft_strlen(current->value)) == 0 && \
			ft_strlen(current->value) == 1 && \
			next->token_type == TYPE_CONTROL_OPERATOR && \
			next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN);
}

int	subshell_open_count(t_list *tokens)
{
	int		open_count;
	t_list	*current;
	t_token	*token;

	open_count = 0;
	current = tokens;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
			open_count++;
		current = current->next;
	}
	return (open_count);
}

int	subshell_closed_count(t_list *tokens)
{
	int			close_count;
	t_list		*current;
	t_token		*token;

	close_count = 0;
	current = tokens;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
			close_count++;
		current = current->next;
	}
	return (close_count);
}

bool	check_subshell_balance(t_list *tokens)
{
	int	open_count;
	int	close_count;

	open_count = subshell_open_count(tokens);
	close_count = subshell_closed_count(tokens);
	if (open_count != close_count)
	{
		if (open_count > close_count)
			print_syntax_error("newline");
		else
			print_syntax_error(")");
		return (false);
	}
	return (true);
}
