/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:07 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/23 18:51:43 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../lexer/lexer.h"

typedef struct s_ast_node
{
	struct s_ast_node *left;
	struct s_ast_node *right;
	t_token *value;
} t_ast_node;

bool	parsing(t_list *tokens);

void	print_syntax_error(char *token_value);

bool syntax_analyze(t_list *tokens);

bool check_token_syntax(t_list *current_token);

bool check_subshell_syntax(t_list *current_token);
bool check_subshell_balance(t_list *tokens);
bool is_dollar_before_subshell(t_token *current, t_list *tokens);

bool check_ctrl_operator_syntax(t_list *current_token);

bool check_operator_combinations_for_redirection(t_list *tokens);
bool check_here_doc_count(t_list *tokens);

bool check_closed_quotes(t_list *tokens);

t_ast_node **parse_logicale_operator(t_token **current_token);
t_ast_node **parse_pipeline(t_token **current_token);
t_ast_node **parse_command(t_token **current_token);
t_ast_node **parse_subshell(t_token **current_token);
t_ast_node **parse_redirection(t_token **current_token);

#endif
