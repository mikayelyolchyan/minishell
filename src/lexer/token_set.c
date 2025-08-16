/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:43:08 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/16 23:51:46 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer/lexer.h"

void	set_operator(t_token *new_token, const char *line, size_t *index)
{
	new_token->type = TYPE_OPERATOR;
	if (line[*index] == '|' && line[*index + 1] == '|')
		new_token->op_type = OP_OR;
	else if (line[*index] == '&' && line[*index + 1] == '&')
		new_token->op_type = OP_AND;
	else if (line[*index] == '|')
		new_token->op_type = OP_PIPE;
	else if (line[*index] == '&')
		new_token->op_type = OP_BACKGROUND;
	else if (line[*index] == '(')
		new_token->op_type = OP_SUBSHELL_OPEN;
	else if (line[*index] == ')')
		new_token->op_type = OP_SUBSHELL_CLOSE;
	else if (line[*index] == ';')
		new_token->op_type = OP_END;
	if (new_token->op_type == OP_OR || new_token->op_type == OP_AND)
		*index += 2;
	else
		*index += 1;
}

void	set_redirection(t_token *new_token, const char *line, size_t *index)
{
	new_token->type = TYPE_REDIRECTION;
	if (line[*index] == '>' && line[*index + 1] == '>')
		new_token->redir_type = REDIR_APPEND;
	else if (line[*index] == '>')
		new_token->redir_type = REDIR_OUT;
	else if (line[*index] == '<' && line[*index + 1] == '<')
		new_token->redir_type = REDIR_HEREDOC;
	else if (line[*index] == '<')
		new_token->redir_type = REDIR_IN;
	if (new_token->redir_type == REDIR_APPEND || \
		new_token->redir_type == REDIR_HEREDOC)
		*index += 2;
	else
		*index += 1;
}
