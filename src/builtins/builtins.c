/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:02:33 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/15 18:12:50 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins/builtins.h"

static int	execute_builtin_cd_pwd(char *cmd, t_ast_node *node,
	t_shell *shell)
{
	if (ft_strncmp(cmd, "cd", 2) == 0 && cmd[2] == '\0')
		return (builtin_cd(node->command->argument, shell));
	else if (ft_strncmp(cmd, "pwd", 3) == 0 && cmd[3] == '\0')
		return (builtin_pwd());
	return (-1);
}

static int	execute_builtin_env_export(char *cmd, t_ast_node *node,
	t_shell *shell)
{
	if (ft_strncmp(cmd, "env", 3) == 0 && cmd[3] == '\0')
		return (builtin_env(shell));
	else if (ft_strncmp(cmd, "export", 6) == 0 && cmd[6] == '\0')
		return (builtin_export(node->command->argument, shell));
	return (-1);
}

int	execute_builtin(t_ast_node *node, t_shell *shell)
{
	char	*cmd;
	int		result;

	if (!node || !node->command || !node->command->argument)
		return (1);
	cmd = node->command->argument[0];
	if (!cmd)
		return (1);
	if (ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0')
		return (builtin_echo(node->command->argument));
	result = execute_builtin_cd_pwd(cmd, node, shell);
	if (result != -1)
		return (result);
	result = execute_builtin_env_export(cmd, node, shell);
	if (result != -1)
		return (result);
	if (ft_strncmp(cmd, "unset", 5) == 0 && cmd[5] == '\0')
		return (builtin_unset(node->command->argument, shell));
	else if (ft_strncmp(cmd, "exit", 4) == 0 && cmd[4] == '\0')
		return (builtin_exit(node->command->argument, shell));
	return (1);
}
