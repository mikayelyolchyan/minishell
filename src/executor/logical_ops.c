/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 18:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"

bool	execute_and(t_ast_node *ast, t_shell *shell)
{
	if (!ast || !shell)
		return (false);
	if (ast->left)
	{
		if (ast->left->command)
			execute_command(ast->left, shell);
		else
			execute_pipe(ast->left, shell);
	}
	if (shell->last_exit_status == 0 && ast->right)
	{
		if (ast->right->command)
			execute_command(ast->right, shell);
		else
			execute_pipe(ast->right, shell);
	}
	return (true);
}

bool	execute_or(t_ast_node *ast, t_shell *shell)
{
	if (!ast || !shell)
		return (false);
	if (ast->left)
	{
		if (ast->left->command)
			execute_command(ast->left, shell);
		else
			execute_pipe(ast->left, shell);
	}
	if (shell->last_exit_status != 0 && ast->right)
	{
		if (ast->right->command)
			execute_command(ast->right, shell);
		else
			execute_pipe(ast->right, shell);
	}
	return (true);
}

static void	execute_subshell_child(t_ast_node *ast, t_shell *shell)
{
	if (ast->left)
	{
		if (ast->left->command)
			execute_command(ast->left, shell);
		else
			execute_pipe(ast->left, shell);
	}
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
