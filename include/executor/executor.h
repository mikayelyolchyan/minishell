/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:00:58 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/03 17:43:34 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include "../env/env.h"
# include "../parser/parser.h"

bool	execute_ast(t_ast_node *ast, t_shell *shell);
bool	is_bulit_in_cmd(t_ast_node *cmd_node);
char	*check_if_cmd_is_path(char *cmd);
char	*find_path(t_env *env_list);
char	**split_env_value(char *env_value);
char	*find_cmd_path(char *cmd_name, t_env *env_list);
void	check_file_access(char *cmd_path);
int		execute_command(t_ast_node *ast, t_shell *shell);
void	execute_command_child(t_ast_node *cmd_node, char *cmd_path,
			t_shell *shell);
void	free_arr(char **arr);
void	shift_empty_args(char **args);
void	clean_empty_markers(char **args);
int		handle_empty_command(t_shell *shell);
int		process_empty_args(t_ast_node *cmd_node, t_shell *shell);
int		execute_builtin(t_ast_node *node, t_shell *shell);
void	execute_cmd_child(t_ast_node *cmd_node, char *cmd_path,
			t_shell *shell);
int		handle_single_redir(t_redir *r, t_shell *shell);
int		apply_redir(t_ast_node *cmd_node, t_shell *shell);
int		execute_builtin_with_redir(t_ast_node *cmd_node, t_shell *shell);
int		execute_empty_cmd(t_ast_node *cmd_node, t_shell *shell);
int		execute_builtin_cmd(t_ast_node *cmd_node, t_shell *shell);
int		execute_pipe(t_ast_node *ast, t_shell *shell);
bool	execute_and(t_ast_node *ast, t_shell *shell);
bool	execute_or(t_ast_node *ast, t_shell *shell);
bool	execute_subshell(t_ast_node *ast, t_shell *shell);

#endif