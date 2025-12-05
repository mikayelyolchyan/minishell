/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 00:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 00:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"
#include <sys/stat.h>

static void	check_is_directory(char *cmd_path)
{
	struct stat	path_stat;

	if (stat(cmd_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		write(2, "minishell: ", 11);
		write(2, cmd_path, ft_strlen(cmd_path));
		write(2, ": Is a directory\n", 17);
		free(cmd_path);
		exit(126);
	}
}

void	check_file_access(char *cmd_path)
{
	if (access(cmd_path, F_OK) != 0)
	{
		write(2, "minishell: ", 11);
		write(2, cmd_path, ft_strlen(cmd_path));
		write(2, ": No such file or directory\n", 28);
		free(cmd_path);
		exit(127);
	}
	check_is_directory(cmd_path);
	if (access(cmd_path, X_OK) != 0)
	{
		write(2, "minishell: ", 11);
		write(2, cmd_path, ft_strlen(cmd_path));
		write(2, ": Permission denied\n", 20);
		free(cmd_path);
		exit(126);
	}
}
