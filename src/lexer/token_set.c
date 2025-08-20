/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:43:08 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/20 20:13:31 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer/lexer.h"
#include "../../include/lexer/utils.h"

static bool	handle_quoted_char(const char *line, size_t i, char *quote)
{
	if (*quote == '\'')
	{
		if (line[i] == '\'')
			*quote = '\0';
	}
	else if (*quote == '"')
	{
		if (line[i] == '"')
			*quote = '\0';
	}
	else
	{
		if (is_space(line[i]) || is_operator(line[i]) || \
			is_redirection(line[i]))
			return (false);
		else if (line[i] == '\'' || line[i] == '"')
			*quote = line[i];
	}
	return (true);
}

static size_t	get_word_end(const char *line, size_t i, bool *is_closed)
{
	char	quote;

	quote = '\0';
	*is_closed = true;
	while (line[i])
	{
		if (handle_quoted_char(line, i, &quote) == false)
			break ;
		i++;
	}
	if (quote != '\0')
		*is_closed = false;
	return (i);
}

bool	set_word(t_token *new_token, const char *line, size_t *index)
{
	size_t		start;
	size_t		end;
	bool		is_closed;

	start = *index;
	end = get_word_end(line, start, &is_closed);
	if (!is_closed)
		return (false);
	new_token->type = TYPE_WORD;
	new_token->value = ft_substr(line, start, end - start);
	if (new_token->value == NULL)
		return (false);
	*index = end;
	return (true);
}

void	set_control_operator(t_token *new_token, const char *line, \
								size_t *index)
{
	new_token->type = TYPE_CONTROL_OPERATOR;
	if (line[*index] == '|' && line[*index + 1] == '|')
		new_token->op_type = CTRL_OP_OR;
	else if (line[*index] == '&' && line[*index + 1] == '&')
		new_token->op_type = CTRL_OP_AND;
	else if (line[*index] == '|')
		new_token->op_type = CTRL_OP_PIPE;
	else if (line[*index] == '&')
		new_token->op_type = CTRL_OP_BACKGROUND;
	else if (line[*index] == '(')
		new_token->op_type = CTRL_OP_SUBSHELL_OPEN;
	else if (line[*index] == ')')
		new_token->op_type = CTRL_OP_SUBSHELL_CLOSE;
	else if (line[*index] == ';')
		new_token->op_type = CTRL_OP_END;
	if (new_token->op_type == CTRL_OP_OR || new_token->op_type == CTRL_OP_AND)
		*index += 2;
	else
		*index += 1;
	new_token->value = NULL;
}

void	set_redirection_operator(t_token *new_token, const char *line, \
									size_t *index)
{
	new_token->type = TYPE_REDIRECTION_OPERATOR;
	if (line[*index] == '>' && line[*index + 1] == '>')
		new_token->redir_type = REDIR_OP_APPEND;
	else if (line[*index] == '>')
		new_token->redir_type = REDIR_OP_OUT;
	else if (line[*index] == '<' && line[*index + 1] == '<')
		new_token->redir_type = REDIR_OP_HERE_DOC;
	else if (line[*index] == '<')
		new_token->redir_type = REDIR_OP_IN;
	if (new_token->redir_type == REDIR_OP_APPEND || \
		new_token->redir_type == REDIR_OP_HERE_DOC)
		*index += 2;
	else
		*index += 1;
	new_token->value = NULL;
}
