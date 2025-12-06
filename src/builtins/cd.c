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
#include <unistd.h>

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

static char	*get_oldpwd(t_shell *shell)
{
	t_env	*current;

	current = shell->env_list;
	while (current)
	{
		if (ft_strncmp(current->name, "OLDPWD", 6) == 0
			&& current->name[6] == '\0')
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
		return (ft_putendl_fd("minishell: cd: too many arguments", 2),
			*error = 1, NULL);
	if (!args[1] || (args[1][0] == '~' && args[1][1] == '\0'))
	{
		path = get_home_dir(shell);
		if (!path)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2),
				*error = 1, NULL);
		return (path);
	}
	if (args[1][0] == '-' && args[1][1] == '\0')
	{
		path = get_oldpwd(shell);
		if (!path)
			return (ft_putendl_fd("minishell: cd: OLDPWD not set", 2),
				*error = 1, NULL);
		return (path);
	}
	return (args[1]);
}

static int	change_directory(char *path)
{
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

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	old_cwd[4096];
	char	new_cwd[4096];
	int		error;

	path = get_target_path(args, shell, &error);
	if (error || !path || path[0] == '\0')
		return (error);
	if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
		old_cwd[0] = '\0';
	if (change_directory(path))
		return (1);
	if (old_cwd[0] != '\0')
		set_env_value(shell->env_list, "OLDPWD", old_cwd);
	if (getcwd(new_cwd, sizeof(new_cwd)) != NULL)
		set_env_value(shell->env_list, "PWD", new_cwd);
	update_env_array(shell);
	if (args[1] && args[1][0] == '-' && args[1][1] == '\0')
		ft_putendl_fd(path, STDOUT_FILENO);
	return (0);
}
