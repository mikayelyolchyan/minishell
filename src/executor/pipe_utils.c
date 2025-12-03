/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 18:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"

static void	execute_left_pipe(t_ast_node *ast, t_shell *shell, int *pipefd)
{
	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		exit(1);
	}
	close(pipefd[1]);
	if (ast->left->command)
		exit(execute_command(ast->left, shell));
	else
		exit(execute_pipe(ast->left, shell));
}

static void	execute_right_pipe(t_ast_node *ast, t_shell *shell, int *pipefd)
{
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		exit(1);
	}
	close(pipefd[0]);
	if (ast->right->command)
		exit(execute_command(ast->right, shell));
	else
		exit(execute_pipe(ast->right, shell));
}

static int	wait_for_children(pid_t pid_left, pid_t pid_right, t_shell *shell)
{
	int	status;
	int	exit_code;

	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else
		exit_code = 1;
	shell->last_exit_status = exit_code;
	return (exit_code);
}

int	execute_pipe(t_ast_node *ast, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), 1);
	pid_left = fork();
	if (pid_left == -1)
		return (perror("minishell: fork"), close(pipefd[0]),
			close(pipefd[1]), 1);
	if (pid_left == 0)
		execute_left_pipe(ast, shell, pipefd);
	pid_right = fork();
	if (pid_right == -1)
		return (perror("minishell: fork"), close(pipefd[0]),
			close(pipefd[1]), waitpid(pid_left, NULL, 0), 1);
	if (pid_right == 0)
		execute_right_pipe(ast, shell, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	return (wait_for_children(pid_left, pid_right, shell));
}
