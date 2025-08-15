/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:04 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/15 21:53:49 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

// || | &&        operator
// >> < > <<   	  redir
// ls pwd cd      word

typedef enum e_token_type
{
	OPERATOR,
	REDIRECTION,
	WORD
}	t_token_type;

typedef enum e_operator_type
{
	PIPE,
	AND,
	OR
}	t_operator_type;

typedef enum e_redirection_type
{
	IN,
	OUT,
	APPEND,
	HEREDOC
}	t_redirection_type;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
	t_operator_type			op_type;
	t_redirection_type		redir_type;
	struct s_token			*next;
}	t_token;

#endif