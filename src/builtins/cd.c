/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins/builtins.h"
#include <string.h>
#include <errno.h>

static char	*get_home_dir(t_shell *shell)
{
	t_env	*current;

	current = shell->env_list;
	while (current)
	{
		if (ft_strncmp(current->name, "HOME", 4) == 0
			&& current->name[4] == '\0')
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static char	*get_target_path(char **args, t_shell *shell, int *error)
{
	char	*path;

	*error = 0;
	if (args[1] && args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		*error = 1;
		return (NULL);
	}
	if (!args[1] || (args[1][0] == '~' && args[1][1] == '\0'))
	{
		path = get_home_dir(shell);
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			*error = 1;
		}
		return (path);
	}
	return (args[1]);
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	int		error;

	path = get_target_path(args, shell, &error);
	if (error)
		return (1);
	if (!path)
		return (0);
	if (path[0] == '\0')
		return (0);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (1);
	}
	return (0);
}
