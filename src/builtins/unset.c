/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins/builtins.h"
#include <unistd.h>

static void	free_env_node(t_env *node)
{
	if (!node)
		return ;
	if (node->name)
		free(node->name);
	if (node->value)
		free(node->value);
	free(node);
}

static int	unset_variable(t_shell *shell, char *name)
{
	t_env	*current;
	t_env	*prev;
	size_t	name_len;

	if (!is_valid_identifier(name))
	{
		ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
		ft_putstr_fd(name, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	current = shell->env_list;
	prev = NULL;
	name_len = ft_strlen(name);
	while (current)
	{
		if (ft_strncmp(current->name, name, name_len) == 0
			&& current->name[name_len] == '\0')
		{
			if (prev)
				prev->next = current->next;
			else
				shell->env_list = current->next;
			free_env_node(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

int	builtin_unset(char **args, t_shell *shell)
{
	int	i;
	int	ret;

	if (!args[1])
		return (0);
	i = 1;
	ret = 0;
	while (args[i])
	{
		if (unset_variable(shell, args[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
