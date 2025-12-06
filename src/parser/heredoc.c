/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 20:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 20:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"
#include "../../include/expansion/expansion.h"
#include "../../include/signals/signals.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>

static bool	handle_heredoc_line(int tmp_fd, char *line, const char *delimiter)
{
	if (ft_strncmp(delimiter, line, ft_strlen(delimiter) + 1) == 0)
	{
		free(line);
		return (false);
	}
	if (!write_line_to_file(tmp_fd, line))
	{
		perror("minishell: heredoc write");
		free(line);
		return (false);
	}
	free(line);
	return (true);
}

static void	read_heredoc_loop(int tmp_fd, const char *delimiter)
{
	char	*line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			line = read_heredoc_line_from_stdin();
		if (!line)
		{
			print_heredoc_warning(delimiter);
			break ;
		}
		if (!handle_heredoc_line(tmp_fd, line, delimiter))
			break ;
	}
}

static char	*get_heredoc_delimiter(t_redir *redir, t_shell *shell)
{
	char	*delimiter;

	delimiter = expand_variables(redir->filename, shell);
	if (!delimiter)
		delimiter = ft_strdup(redir->filename);
	return (delimiter);
}

bool	make_heredoc(t_redir *redir, t_shell *shell, int heredoc_index)
{
	char		*tmp_filename;
	int			tmp_fd;
	char		*delimiter;

	if (!redir || !redir->filename)
		return (false);
	delimiter = get_heredoc_delimiter(redir, shell);
	tmp_filename = generate_heredoc_filename(heredoc_index);
	if (!tmp_filename)
		return (free(delimiter), false);
	tmp_fd = open_heredoc_file(tmp_filename);
	if (tmp_fd < 0)
		return (free(delimiter), false);
	if (isatty(STDIN_FILENO))
		setup_signals_heredoc();
	read_heredoc_loop(tmp_fd, delimiter);
	if (isatty(STDIN_FILENO))
		setup_signals_interactive();
	close(tmp_fd);
	free(delimiter);
	redir->heredoc_tmpfile = tmp_filename;
	return (true);
}
