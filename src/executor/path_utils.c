/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"
#include <unistd.h>

static void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*get_env_value(t_env *env_list, char *name)
{
	t_env	*current;
	size_t	name_len;

	if (!env_list || !name)
		return (NULL);
	current = env_list;
	name_len = ft_strlen(name);
	while (current)
	{
		if (ft_strncmp(current->name, name, name_len) == 0
			&& current->name[name_len] == '\0')
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static char	*join_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full_path);
}

static char	*search_in_path(char **dirs, char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (dirs[i])
	{
		full_path = join_path(dirs[i], cmd);
		if (!full_path)
		{
			i++;
			continue ;
		}
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_executable(char *cmd, t_env *env_list)
{
	char	**dirs;
	char	*path;
	char	*result;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path = get_env_value(env_list, "PATH");
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	result = search_in_path(dirs, cmd);
	free_array(dirs);
	return (result);
}
