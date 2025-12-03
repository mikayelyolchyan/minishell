/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 20:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 20:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer/lexer.h"
#include "../../include/lexer/utils.h"

bool	handle_quoted_char(const char *line, size_t i, char *quote)
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
		if (is_space(line[i]) || is_operator(line[i])
			|| is_redirection(line[i]))
			return (false);
		else if (line[i] == '\'' || line[i] == '"')
			*quote = line[i];
	}
	return (true);
}

size_t	get_word_end(const char *line, size_t i)
{
	char	quote;

	quote = '\0';
	while (line[i])
	{
		if (handle_quoted_char(line, i, &quote) == false)
			break ;
		i++;
	}
	return (i);
}

void	determine_ctrl_op_type(t_token *new_token, const char *line,
	size_t index)
{
	if (line[index] == '|' && line[index + 1] == '|')
		new_token->ctrl_op_type = CTRL_OP_OR;
	else if (line[index] == '&' && line[index + 1] == '&')
		new_token->ctrl_op_type = CTRL_OP_AND;
	else if (line[index] == '|')
		new_token->ctrl_op_type = CTRL_OP_PIPE;
	else if (line[index] == '&')
		new_token->ctrl_op_type = CTRL_OP_BACKGROUND;
	else if (line[index] == '(')
		new_token->ctrl_op_type = CTRL_OP_SUBSHELL_OPEN;
	else if (line[index] == ')')
		new_token->ctrl_op_type = CTRL_OP_SUBSHELL_CLOSE;
	else if (line[index] == ';')
		new_token->ctrl_op_type = CTRL_OP_END;
}
