/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins/builtins.h"
#include <unistd.h>

static int	env_list_size(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static t_env	**create_env_array(t_env *env_list, int size)
{
	t_env	**array;
	t_env	*current;
	int		i;

	array = malloc(sizeof(t_env *) * size);
	if (!array)
		return (NULL);
	current = env_list;
	i = 0;
	while (current)
	{
		array[i] = current;
		current = current->next;
		i++;
	}
	return (array);
}

static void	sort_env_array(t_env **array, int size)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (compare_env_names(array[i]->name, array[j]->name) > 0)
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export_line(t_env *env)
{
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(env->name, STDOUT_FILENO);
	if (env->value)
	{
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(env->value, STDOUT_FILENO);
		ft_putstr_fd("\"", STDOUT_FILENO);
	}
	ft_putstr_fd("\n", STDOUT_FILENO);
}

void	print_all_exports(t_env *env_list)
{
	int		size;
	t_env	**array;
	int		i;

	size = env_list_size(env_list);
	if (size == 0)
		return ;
	array = create_env_array(env_list, size);
	if (!array)
		return ;
	sort_env_array(array, size);
	i = 0;
	while (i < size)
	{
		print_export_line(array[i]);
		i++;
	}
	free(array);
}
