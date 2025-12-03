/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 20:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 20:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

char	*read_heredoc_line_from_stdin(void)
{
	char	buffer[1024];
	ssize_t	i;
	ssize_t	bytes_read;
	char	*line;

	i = 0;
	while (i < (ssize_t)(sizeof(buffer) - 1))
	{
		bytes_read = read(STDIN_FILENO, &buffer[i], 1);
		if (bytes_read <= 0)
			break ;
		if (buffer[i] == '\n')
			break ;
		i++;
	}
	if (i == 0 && bytes_read <= 0)
		return (NULL);
	buffer[i] = '\0';
	line = ft_strdup(buffer);
	return (line);
}

int	open_heredoc_file(char *tmp_filename)
{
	int	tmp_fd;

	tmp_fd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (tmp_fd < 0)
	{
		perror("minishell: heredoc");
		free(tmp_filename);
	}
	return (tmp_fd);
}

char	*generate_heredoc_filename(int index)
{
	char	*index_str;
	char	*filename;

	index_str = ft_itoa(index);
	if (!index_str)
		return (NULL);
	filename = ft_strjoin("/tmp/.heredoc_", index_str);
	free(index_str);
	return (filename);
}

void	print_heredoc_warning(const char *delimiter)
{
	write(STDERR_FILENO, "minishell: warning: here-document delimited by ", 48);
	write(STDERR_FILENO, "end-of-file (wanted `", 21);
	write(STDERR_FILENO, delimiter, ft_strlen(delimiter));
	write(STDERR_FILENO, "')\n", 3);
}

bool	apply_heredoc(t_command *cmd, t_shell *shell, int *heredoc_index)
{
	t_redir	*redir;

	if (!cmd)
		return (true);
	redir = cmd->redir;
	while (redir)
	{
		if (redir->redir_type == REDIR_OP_HERE_DOC)
		{
			(*heredoc_index)++;
			if (!make_heredoc(redir, shell, *heredoc_index))
				return (false);
		}
		redir = redir->next;
	}
	return (true);
}
