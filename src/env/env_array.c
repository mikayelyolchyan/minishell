/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 18:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env/env.h"
#include "../../lib/libft/libft.h"

static int	count_env_list(t_env *list)
{
	t_env	*cur;
	int		count;

	count = 0;
	cur = list;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	return (count);
}

static char	*create_env_entry(t_env *node)
{
	char	*entry;
	char	*result;

	entry = ft_strjoin(node->name, "=");
	if (!entry)
		return (NULL);
	if (node->value)
	{
		result = ft_strjoin(entry, node->value);
		free(entry);
		return (result);
	}
	return (entry);
}

char	**env_list_to_array(t_env *list)
{
	char	**array;
	t_env	*cur;
	int		i;

	array = malloc(sizeof(char *) * (count_env_list(list) + 1));
	if (!array)
		return (NULL);
	i = 0;
	cur = list;
	while (cur)
	{
		array[i] = create_env_entry(cur);
		if (!array[i])
		{
			while (--i >= 0)
				free(array[i]);
			free(array);
			return (NULL);
		}
		cur = cur->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}
