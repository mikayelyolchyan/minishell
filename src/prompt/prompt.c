/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:55 by miyolchy          #+#    #+#             */
/*   Updated: 2025/11/30 18:09:21 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/prompt/prompt.h"
#include "../../include/lexer/lexer.h"
#include "../../include/parser/parser.h"
#include "../../include/executor/executor.h"
#include "../../include/signals/signals.h"
#include <unistd.h>

static char	*read_line_from_stdin(void)
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

static void	handle_sigint(t_shell *shell)
{
	if (g_signal == SIGINT)
	{
		shell->last_exit_status = 130;
		g_signal = 0;
	}
}

static bool	process_input(char *line, t_shell *shell)
{
	t_list	*tokens;

	if (line[0] != '\0')
		add_history(line);
	tokens = lexical_analyze(line);
	if (tokens == NULL)
		return (false);
	parsing(tokens, shell);
	ft_lstclear(&tokens, del_token);
	return (true);
}

void	get_prompt_line(t_shell *shell)
{
	char	*line;

	(void)shell;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("minishell$ ");
		else
			line = read_line_from_stdin();
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				write(1, "exit\n", 5);
			break ;
		}
		shell->current_line = line;
		handle_sigint(shell);
		process_input(line, shell);
		free(line);
		shell->current_line = NULL;
		if (shell->should_exit)
			break ;
	}
}
