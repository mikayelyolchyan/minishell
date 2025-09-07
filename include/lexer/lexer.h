/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:04 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/21 19:07:35 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../../lib/libft/libft.h"
# include <stdbool.h>
# include <stdio.h>

typedef enum e_token_type
{
	TYPE_CONTROL_OPERATOR,
	TYPE_REDIRECTION_OPERATOR,
	TYPE_WORD
}	t_token_type;

typedef enum e_control_operator_type
{
	CTRL_OP_NONE,
	CTRL_OP_PIPE,
	CTRL_OP_AND,
	CTRL_OP_OR,
	CTRL_OP_SUBSHELL_OPEN,
	CTRL_OP_SUBSHELL_CLOSE,
	CTRL_OP_END,
	CTRL_OP_BACKGROUND 
}	t_control_operator_type;

/*typedef enum e_control_operator_subshell_type
{
	CTRL_OP_SUBSHELL_NON,
	CTRL_OP_SUBSHELL_OPEN,
	CTRL_OP_SUBSHELL_CLOSE,
} t_control_operator_subshell_type;*/

typedef enum e_redirection_operator_type
{
	REDIR_OP_NONE,
	REDIR_OP_IN,
	REDIR_OP_OUT,
	REDIR_OP_APPEND,
	REDIR_OP_HERE_DOC
}	t_redirection_operator_type;

typedef struct s_token
{
	t_token_type					token_type;
	char							*value;
	t_control_operator_type			ctrl_op_type;
	t_redirection_operator_type		redir_op_type;

}	t_token;

t_list		*lexical_analyze(const char *line);

void		del_token(void *content);

void		set_control_operator(t_token *new_token, const char *line, \
									size_t *index);
void		set_redirection_operator(t_token *new_token, const char *line, \
										size_t *index);
bool		set_word(t_token *new_token, const char *line, size_t *index);

#endif