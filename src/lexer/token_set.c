/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:43:08 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/21 19:08:48 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer/lexer.h"
#include "../../include/lexer/utils.h"

bool	set_word(t_token *new_token, const char *line, size_t *index)
{
	size_t		start;
	size_t		end;

	start = *index;
	end = get_word_end(line, start);
	new_token->token_type = TYPE_WORD;
	new_token->value = ft_substr(line, start, end - start);
	if (new_token->value == NULL)
		return (false);
	*index = end;
	return (true);
}

void	set_control_operator(t_token *new_token, const char *line,
	size_t *index)
{
	size_t	start;

	start = *index;
	new_token->token_type = TYPE_CONTROL_OPERATOR;
	determine_ctrl_op_type(new_token, line, *index);
	if (new_token->ctrl_op_type == CTRL_OP_OR
		|| new_token->ctrl_op_type == CTRL_OP_AND)
		*index += 2;
	else
		*index += 1;
	new_token->value = ft_substr(line, start, *index - start);
	if (new_token->value == NULL)
		return ;
}

void	set_redirection_operator(t_token *new_token, const char *line,
	size_t *index)
{
	size_t	start;

	start = *index;
	new_token->token_type = TYPE_REDIRECTION_OPERATOR;
	if (line[*index] == '>' && line[*index + 1] == '>')
		new_token->redir_op_type = REDIR_OP_APPEND;
	else if (line[*index] == '>')
		new_token->redir_op_type = REDIR_OP_OUT;
	else if (line[*index] == '<' && line[*index + 1] == '<')
		new_token->redir_op_type = REDIR_OP_HERE_DOC;
	else if (line[*index] == '<')
		new_token->redir_op_type = REDIR_OP_IN;
	if (new_token->redir_op_type == REDIR_OP_APPEND
		|| new_token->redir_op_type == REDIR_OP_HERE_DOC)
		*index += 2;
	else
		*index += 1;
	new_token->value = ft_substr(line, start, *index - start);
	if (new_token->value == NULL)
		return ;
}
