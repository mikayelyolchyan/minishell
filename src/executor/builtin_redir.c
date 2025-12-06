/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 00:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/04 00:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"
#include "../../include/builtins/builtins.h"

static int	save_fds(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin < 0 || *saved_stdout < 0)
	{
		if (*saved_stdin >= 0)
			close(*saved_stdin);
		if (*saved_stdout >= 0)
			close(*saved_stdout);
		return (0);
	}
	return (1);
}

static void	restore_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

int	execute_empty_cmd(t_ast_node *cmd_node, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	if (cmd_node->command->redir)
	{
		if (!save_fds(&saved_stdin, &saved_stdout))
			return (1);
		if (!apply_redir(cmd_node, shell))
		{
			restore_fds(saved_stdin, saved_stdout);
			return (1);
		}
		restore_fds(saved_stdin, saved_stdout);
	}
	return (0);
}

int	execute_builtin_cmd(t_ast_node *cmd_node, t_shell *shell)
{
	if (cmd_node->command->redir)
		shell->last_exit_status
			= execute_builtin_with_redir(cmd_node, shell);
	else
		shell->last_exit_status = execute_builtin(cmd_node, shell);
	return (shell->last_exit_status);
}

int	execute_builtin_with_redir(t_ast_node *cmd_node, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	result;

	if (!save_fds(&saved_stdin, &saved_stdout))
		return (1);
	if (!apply_redir(cmd_node, shell))
		return (restore_fds(saved_stdin, saved_stdout), 1);
	result = execute_builtin(cmd_node, shell);
	restore_fds(saved_stdin, saved_stdout);
	return (result);
}
