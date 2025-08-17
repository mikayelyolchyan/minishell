/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:43:08 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/17 20:20:27 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer/lexer.h"
#include "../../include/lexer/utils.h"

static size_t	get_word_end(const char *line, size_t i)
{
	char	quote;

	quote = '\0';
	while (line[i])
	{
		if (quote == '\'')
		{
			if (line[i] == '\'')
				quote = '\0';
		}
		else if (quote == '"')
		{
			if (line[i] == '\\' && (line[i + 1] == '"' || line[i + 1] == '$'
					|| line[i + 1] == '\\'))
				i++;
			else if (line[i] == '"')
				quote = '\0';
		}
		else
		{
			if (line[i] == '\\')
			{
				i++;
				if (!line[i])
					break ;
			}
			else if (is_space(line[i]) || is_operator(line[i])
				|| is_redirection(line[i]))
				break ;
			else if (line[i] == '\'' || line[i] == '"')
				quote = line[i];
		}
		i++;
	}
	return (i);
}

bool	set_word(t_token *new_token, const char *line, size_t *index)
{
	size_t	start;
	size_t	end;

	start = *index;
	end = get_word_end(line, start);
	new_token->type = TYPE_WORD;
	new_token->value = ft_substr(line, start, end - start);
	if (new_token->value == NULL)
		return (false);
	*index = end;
	return (true);
}

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
		new_token->redir_type = REDIR_HERE_DOC;
	else if (line[*index] == '<')
		new_token->redir_type = REDIR_IN;
	if (new_token->redir_type == REDIR_APPEND || \
		new_token->redir_type == REDIR_HERE_DOC)
		*index += 2;
	else
		*index += 1;
}
