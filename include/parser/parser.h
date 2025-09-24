/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:07 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/24 23:07:08 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../lexer/lexer.h"

typedef struct s_ast_node
{
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	t_token				*value;
	struct s_command	*command;
}	t_ast_node;

typedef struct s_redir
{
	char						*filename;
	t_redirection_operator_type	redir_type;
	struct s_redir				*next;	
}	t_redir;

typedef struct s_command
{
	char			**argument;
	struct s_redir	*redir;

}	t_command ;

void	print_syntax_error(char *token_value);
int	subshell_open_count(t_list *tokens);
int	subshell_closed_count(t_list *tokens);
bool check_subshell_balance(t_list *tokens);

bool check_subshell_syntax(t_list *current_token);

bool chek_ctrl_operator_syntax(t_list *current_token);

bool check_token_syntax( t_list *current_token);

bool check_operator_combinations_for_redirection(t_list *tokens);

bool	parsing(t_list *tokens);
bool	syntax_analyze(t_list *tokens);

bool is_redir(t_token *token);
void free_string_array(char **array);
void free_ast(t_ast_node *node);
void free_string_array(char **array);

t_ast_node *create_ast_node(t_token *token);
t_ast_node *ast_pipeline(t_list **current_token);
t_ast_node *ast_logical(t_list **current_token);
t_ast_node *build_ast(t_list *token);
t_ast_node *ast_subshell(t_list **current_token);
bool append_argument(t_command *cmd, const char *arg);
//bool handle_redirection(t_command *cmd, t_list **current_token);
t_ast_node *ast_command(t_list **current_token);
void print_ast_command(t_ast_node *node);
void free_redir_list(t_redir *redir);
void free_command(t_command *cmd);
void print_ast_recursive(t_ast_node *node, int depth);
void print_ast(t_ast_node *ast);
t_ast_node *init_command_node(void);
bool process_word_token(t_ast_node *node, t_command *cmd,  t_token *token, bool *has_command);
void parse_command_tokens(t_ast_node *node, t_command *cmd, t_list **current_token);
t_ast_node *build_command_node(t_list **current_token);
bool handle_redirection(t_command *cmd, t_list **current_token);
t_redir *init_redirection(t_token *token);
bool assign_redirection_filename(t_redir *new_redir, t_list **current_token);
void insert_redirection(t_command *cmd, t_redir *new_redir);
#endif
