/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 18:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env/env.h"
#include "../../lib/libft/libft.h"

void	set_env_value(t_env *list, char *name, char *value)
{
	t_env	*cur;

	cur = list;
	while (cur)
	{
		if (ft_strncmp(cur->name, name, ft_strlen(name)) == 0
			&& cur->name[ft_strlen(name)] == '\0')
		{
			if (cur->value)
				free(cur->value);
			cur->value = ft_strdup(value);
			return ;
		}
		cur = cur->next;
	}
}
