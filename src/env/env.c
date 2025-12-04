/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:02:30 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/21 14:26:03 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env/env.h"
#include "../../lib/libft/libft.h"

void	init_shell(t_shell *shell, t_env *env_list)
{
	shell->env_list = env_list;
	shell->env_array = NULL;
	shell->last_exit_status = 0;
	shell->should_exit = 0;
}

t_env	*init_node_env(char *name, char *value)
{
	t_env	*new_nod;

	new_nod = malloc(sizeof(t_env));
	if (!new_nod)
		return (NULL);
	new_nod->name = name;
	new_nod->value = value;
	new_nod->next = NULL;
	return (new_nod);
}

void	append_node_env(t_env **list, t_env *new_node)
{
	t_env	*cur;

	if (!*list)
	{
		*list = new_node;
		return ;
	}
	cur = *list;
	while (cur->next)
		cur = cur->next;
	cur->next = new_node;
}

t_env	*get_env_list(char **envp)
{
	int		i;
	char	*name;
	char	*value;
	t_env	*list;

	i = 0;
	list = NULL;
	while (envp[i])
	{
		split_env_entry(envp[i], &name, &value);
		append_node_env(&list, init_node_env(name, value));
		i++;
	}
	return (list);
}

void	free_env_list(t_env *list)
{
	t_env	*current;
	t_env	*next;

	current = list;
	while (current)
	{
		next = current->next;
		if (current->name)
			free(current->name);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}
