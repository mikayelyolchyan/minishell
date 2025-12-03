/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 18:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env/env.h"
#include "../../lib/libft/libft.h"

char	*dup_substr(const char *src, int len)
{
	char	*dst;
	int		i;

	i = 0;
	dst = malloc(len + 1);
	if (!dst)
		return (NULL);
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

void	split_env_entry(char *entry, char **name, char **value)
{
	int	i;

	i = 0;
	while (entry[i] && entry[i] != '=')
		i++;
	if (entry[i] == '\0')
	{
		*name = ft_strdup(entry);
		*value = NULL;
	}
	else
	{
		*name = dup_substr(entry, i);
		*value = ft_strdup(entry + i + 1);
	}
}

void	print_env_list(t_env *list)
{
	t_env	*cur;

	cur = list;
	while (cur)
	{
		if (cur->value)
			printf("%s=%s\n", cur->name, cur->value);
		else
			printf("%s=\n", cur->name);
		cur = cur->next;
	}
}
