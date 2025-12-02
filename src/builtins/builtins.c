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

/*
	Модуль builtins предназначен для реализации встроенных
		команд оболочки minishell.
	
	Встроенные команды (builtins) — это такие команды, которые выполняются
		непосредственно внутри самой оболочки, без запуска внешних программ
		через fork/execve. Это необходимо для команд, которые должны изменять
		состояние самой оболочки (например, cd меняет текущую директорию
		процесса, export/ unset — переменные окружения,
		exit завершает работу оболочки).
	
	В этом модуле реализуются следующие команды:
	- cd: смена текущей рабочей директории
	- echo: вывод текста в стандартный вывод
	- pwd: вывод текущей рабочей директории
	- export: добавление/изменение переменных окружения
	- unset: удаление переменных окружения
	- env: вывод всех переменных окружения
	- exit: завершение работы оболочки
	
	Особенности:
	- Некоторые команды (cd, export, unset, exit) должны выполняться в основном
		процессе, чтобы изменения отражались на самой оболочке, а не только в
		дочернем процессе.
	- Модуль должен предоставлять интерфейс для проверки, является ли команда
		встроенной, и для вызова соответствующей функции.
	- Встроенные команды должны корректно обрабатывать аргументы и возвращать
		коды ошибок, совместимые с bash.
*/

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
