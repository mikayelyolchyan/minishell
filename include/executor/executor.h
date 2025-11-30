/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:00:58 by miyolchy          #+#    #+#             */
/*   Updated: 2025/11/30 17:13:56 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct s_env
{
	//bool exported;
	char *name;
	char *value;
	struct s_env *next;
} t_env ;

typedef struct s_shell
{
    t_env   *env_list;          
    char    **env_array;        
    int      last_exit_status;  
    int      should_exit;      
}   t_shell;

#include "../parser/parser.h"

void init_shell(t_shell *shell, t_env *env_list);
t_env *init_node_env(char *name, char *value);
void append_node_env(t_env **list, t_env *new_node);
char *dup_substr(const char *src, int len);
void split_env_entry(char *entry, char **name, char **value);
t_env *get_env_list(char **envp);
void print_env_list(t_env *list);
bool execute_ast(t_ast_node *ast, t_shell *shell);
bool is_bulit_in_cmd(t_ast_node *cmd_node);
char	*check_if_cmd_is_path(char *cmd);
char *find_path(t_env *env_list);
char **split_env_value(char *env_value);
char *find_cmd_path(char *cmd_name, t_env *env_list);
int execute_command(t_ast_node *ast, t_shell *shell);
void execute_command_child(t_ast_node *cmd_node, char *cmd_path, t_shell *shell);
int	ft_strcmp(const char *s1, const char *s2);
void free_arr(char **arr);
int     execute_builtin(t_ast_node *node, t_shell *shell);
void    execute_cmd_child(t_ast_node *cmd_node, char *cmd_path, t_shell *shell);
bool    handle_cmd_redir(t_ast_node *ast, t_shell *shell);

#endif