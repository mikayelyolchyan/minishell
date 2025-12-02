/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins/builtins.h"

int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	*find_equals_sign(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '=')
			return (&arg[i]);
		i++;
	}
	return (NULL);
}

t_env	*find_env_variable(t_env *env_list, char *name)
{
	t_env	*current;
	size_t	name_len;

	current = env_list;
	name_len = ft_strlen(name);
	while (current)
	{
		if (ft_strncmp(current->name, name, name_len) == 0
			&& current->name[name_len] == '\0')
			return (current);
		current = current->next;
	}
	return (NULL);
}

int	update_or_add_env_var(t_shell *shell, char *name, char *value)
{
	t_env	*var;
	t_env	*new_var;

	var = find_env_variable(shell->env_list, name);
	if (var)
	{
		if (value)
		{
			free(var->value);
			var->value = ft_strdup(value);
		}
		return (0);
	}
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (1);
	new_var->name = ft_strdup(name);
	new_var->value = value ? ft_strdup(value) : NULL;
	new_var->next = NULL;
	append_node_env(&shell->env_list, new_var);
	return (0);
}

int	compare_env_names(char *name1, char *name2)
{
	int	len1;
	int	len2;
	int	max_len;

	len1 = ft_strlen(name1);
	len2 = ft_strlen(name2);
	if (len1 > len2)
		max_len = len1;
	else
		max_len = len2;
	return (ft_strncmp(name1, name2, max_len + 1));
}
