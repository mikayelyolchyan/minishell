/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 21:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 21:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"
#include <unistd.h>

bool	write_line_to_file(int fd, const char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (write(fd, line, len) < 0)
		return (false);
	if (write(fd, "\n", 1) < 0)
		return (false);
	return (true);
}

bool	prepare_heredocs_recursive(t_ast_node *node, t_shell *shell,
	int *heredoc_index)
{
	if (!node)
		return (true);
	if (node->command)
		return (apply_heredoc(node->command, shell, heredoc_index));
	if (!prepare_heredocs_recursive(node->left, shell, heredoc_index))
		return (false);
	if (!prepare_heredocs_recursive(node->right, shell, heredoc_index))
		return (false);
	return (true);
}
