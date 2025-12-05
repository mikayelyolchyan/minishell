/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_interpretor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:41:22 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 20:15:45 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"
#include "../../include/builtins/builtins.h"
#include "../../include/signals/signals.h"
#include "../../include/expansion/expansion.h"

bool	is_bulit_in_cmd(t_ast_node *cmd_node)
{
	char	*cmd;

	if (!cmd_node || !cmd_node->command || !cmd_node->command->argument)
		return (false);
	cmd = cmd_node->command->argument[0];
	if (!cmd)
		return (false);
	if (ft_strncmp(cmd, "cd", 2) == 0 && cmd[2] == '\0')
		return (true);
	else if (ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0')
		return (true);
	else if (ft_strncmp(cmd, "env", 3) == 0 && cmd[3] == '\0')
		return (true);
	else if (ft_strncmp(cmd, "exit", 4) == 0 && cmd[4] == '\0')
		return (true);
	else if (ft_strncmp(cmd, "export", 6) == 0 && cmd[6] == '\0')
		return (true);
	else if (ft_strncmp(cmd, "pwd", 3) == 0 && cmd[3] == '\0')
		return (true);
	else if (ft_strncmp(cmd, "unset", 5) == 0 && cmd[5] == '\0')
		return (true);
	return (false);
}

void	execute_cmd_child(t_ast_node *cmd_node, char *cmd_path, t_shell *shell)
{
	setup_signals_child();
	if (cmd_node->command->redir)
	{
		if (!apply_redir(cmd_node, shell))
		{
			free(cmd_path);
			exit(1);
		}
	}
	check_file_access(cmd_path);
	execve(cmd_path, cmd_node->command->argument, shell->env_array);
	free(cmd_path);
	exit(127);
}

static void	handle_parent_process(pid_t pid, char *cmd_path, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->last_exit_status = 128 + WTERMSIG(status);
	free(cmd_path);
}

static int	execute_external_cmd(t_ast_node *cmd_node, t_shell *shell)
{
	char	*cmd_path;
	pid_t	pid;

	cmd_path = find_cmd_path(cmd_node->command->argument[0], shell->env_list);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_node->command->argument[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		shell->last_exit_status = 127;
		return (shell->last_exit_status);
	}
	pid = fork();
	if (pid < 0)
	{
		free(cmd_path);
		return (write(STDERR_FILENO, "minishell: fork failed\n", 23), 1);
	}
	if (pid == 0)
		execute_cmd_child(cmd_node, cmd_path, shell);
	else
		handle_parent_process(pid, cmd_path, shell);
	return (shell->last_exit_status);
}

int	execute_command(t_ast_node *ast, t_shell *shell)
{
	t_ast_node	*cmd_node;
	int			result;

	cmd_node = ast;
	if (!cmd_node || !cmd_node->command)
		return (1);
	if (cmd_node->command)
	{
		expand_arguments(cmd_node->command->argument, shell);
		expand_redirections(cmd_node->command->redir, shell);
	}
	if (!cmd_node->command->argument || !cmd_node->command->argument[0])
		return (execute_empty_cmd(cmd_node, shell));
	if (cmd_node->command->argument[0][0] == 1)
		return (handle_empty_command(shell));
	if (cmd_node->command->argument[0][0] == '\0')
	{
		result = process_empty_args(cmd_node, shell);
		if (result != -1)
			return (result);
	}
	clean_empty_markers(cmd_node->command->argument);
	if (is_bulit_in_cmd(cmd_node) == true)
		return (execute_builtin_cmd(cmd_node, shell));
	return (execute_external_cmd(cmd_node, shell));
}
