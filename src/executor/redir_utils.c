/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 18:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"
#include <fcntl.h>

static int	open_redir_file(t_redir *r, const char *filename)
{
	int	fd;

	if (r->redir_type == REDIR_OP_IN || r->redir_type == REDIR_OP_HERE_DOC)
		fd = open(filename, O_RDONLY);
	else if (r->redir_type == REDIR_OP_OUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (r->redir_type == REDIR_OP_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	return (fd);
}

static int	dup_redir_fd(t_redir *r, int fd, t_shell *shell)
{
	if (r->redir_type == REDIR_OP_IN || r->redir_type == REDIR_OP_HERE_DOC)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("minishell");
			close(fd);
			shell->last_exit_status = 1;
			return (0);
		}
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("minishell");
			close(fd);
			shell->last_exit_status = 1;
			return (0);
		}
	}
	close(fd);
	return (1);
}

int	handle_single_redir(t_redir *r, t_shell *shell)
{
	int			fd;
	const char	*filename;

	if (!r)
		return (0);
	if (r->redir_type == REDIR_OP_HERE_DOC)
		filename = r->heredoc_tmpfile;
	else
		filename = r->filename;
	if (!filename)
	{
		fprintf(stderr, "minishell: error: redirection filename is NULL\n");
		shell->last_exit_status = 1;
		return (0);
	}
	fd = open_redir_file(r, filename);
	if (fd < 0)
	{
		perror("minishell");
		shell->last_exit_status = 1;
		return (0);
	}
	return (dup_redir_fd(r, fd, shell));
}

int	apply_redir(t_ast_node *cmd_node, t_shell *shell)
{
	t_redir	*redir;

	redir = cmd_node->command->redir;
	while (redir)
	{
		if (!handle_single_redir(redir, shell))
			return (0);
		redir = redir->next;
	}
	return (1);
}
