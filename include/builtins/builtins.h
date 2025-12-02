/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:00:42 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/15 18:00:43 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <unistd.h>
# include <stdio.h>
# include "../executor/executor.h"

int		execute_builtin(t_ast_node *node, t_shell *shell);
int		builtin_echo(char **args);
int		builtin_cd(char **args, t_shell *shell);
int		builtin_pwd(void);
int		builtin_env(t_shell *shell);
int		builtin_export(char **args, t_shell *shell);
int		builtin_unset(char **args, t_shell *shell);
int		builtin_exit(char **args, t_shell *shell);

int		is_valid_identifier(char *name);
char	*find_equals_sign(char *arg);
t_env	*find_env_variable(t_env *env_list, char *name);
int		update_or_add_env_var(t_shell *shell, char *name, char *value);
int		compare_env_names(char *name1, char *name2);
void	print_all_exports(t_env *env_list);

#endif