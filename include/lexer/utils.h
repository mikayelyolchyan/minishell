/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:57:19 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/20 20:19:16 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdbool.h>
# include <stddef.h>
# include "lexer.h"

bool	is_space(int c);
bool	is_operator(int c);
bool	is_redirection(int c);
bool	handle_quoted_char(const char *line, size_t i, char *quote);
size_t	get_word_end(const char *line, size_t i);
void	determine_ctrl_op_type(t_token *new_token, const char *line,
			size_t index);

#endif