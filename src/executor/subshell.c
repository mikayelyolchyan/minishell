/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 21:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 21:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"

static void	execute_subshell_child(t_ast_node *ast, t_shell *shell)
{
	shell->in_subshell = 1;
	if (ast->left)
		execute_ast(ast->left, shell);
	exit(shell->last_exit_status);
}

bool	execute_subshell(t_ast_node *ast, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!ast || !shell)
		return (false);
	status = 0;
	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), false);
	if (pid == 0)
		execute_subshell_child(ast, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->last_exit_status = 128 + WTERMSIG(status);
	return (true);
}
