/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 18:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*check_if_cmd_is_path(char *cmd)
{
	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	return (NULL);
}

char	*find_path(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strncmp(current->name, "PATH", 4) == 0
			&& current->name[4] == '\0')
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static char	*search_in_paths(char **paths, char *cmd_name)
{
	char	*tmp;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd_name);
		free(tmp);
		if (access(full_path, X_OK) == 0)
			return (free_arr(paths), full_path);
		free(full_path);
		i++;
	}
	free_arr(paths);
	return (NULL);
}

char	*find_cmd_path(char *cmd_name, t_env *env_list)
{
	char	**paths;
	char	*path_value;
	char	*full_path;

	if (!cmd_name)
		return (NULL);
	full_path = check_if_cmd_is_path(cmd_name);
	if (full_path)
		return (full_path);
	path_value = find_path(env_list);
	if (!path_value || !path_value[0])
		return (NULL);
	paths = ft_split(path_value, ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd_name));
}
