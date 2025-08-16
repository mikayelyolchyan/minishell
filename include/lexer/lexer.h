/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:04 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/16 21:45:03 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

#include "../../lib/libft/libft.h"

typedef enum e_token_type
{
	TYPE_OPERATOR,
	TYPE_REDIRECTION,
	TYPE_WORD
}	t_token_type;

typedef enum e_operator_type
{
	OP_NONE,
	OP_PIPE,
	OP_AND,
	OP_OR,
	OP_SUBSHELL_OPEN,
	OP_SUBSHELL_CLOSE,
	OP_END,
	OP_BACKGROUND
}	t_operator_type;

typedef enum e_redirection_type
{
	REDIR_NONE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redirection_type;

typedef enum e_quote_type
{
    QUOTE_NONE,
    QUOTE_SINGLE,
    QUOTE_DOUBLE
}	t_quote_type;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
	t_operator_type			op_type;
	t_redirection_type		redir_type;
	t_quote_type			quote_type;
}	t_token;

t_list		*tokenize(const char *line);

t_token		*token_alloc_init(t_token_type type, const char *val);

void		set_operator(t_token *new_token, const char *line, size_t *index);
void		set_redirection(t_token *new_token, const char *line, \
							size_t *index);

#endif