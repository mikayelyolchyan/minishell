/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:07 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/21 21:01:40 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../lexer/lexer.h"

void print_syntax_error(char *token_value);

int subshell_open_count(t_list *tokens);

int subshell_closed_count(t_list *tokens);

bool check_subshell_balance(t_list *tokens);

bool check_subshell_syntax(t_token *current_token, t_token *next_token);

bool chek_ctrl_operator_syntax(t_token *current_token, t_token *next_token);

bool check_token_syntax( t_token *current_token, t_token *next_token);

bool check_operator_combinations_for_redirection(t_list *tokens);

bool	parsing(t_list *tokens);
bool	syntax_analyze(t_list *tokens);

#endif