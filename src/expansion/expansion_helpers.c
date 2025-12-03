/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 00:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/01/07 00:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expansion/expansion.h"
#include "../../include/env/env.h"

char	*expand_tilde(t_shell *shell)
{
	t_env	*env;

	env = shell->env_list;
	while (env)
	{
		if (ft_strncmp(env->name, "HOME", 4) == 0 && env->name[4] == '\0')
		{
			if (env->value)
				return (ft_strdup(env->value));
			return (ft_strdup("~"));
		}
		env = env->next;
	}
	return (ft_strdup("~"));
}

int	should_expand_tilde(char *str, int i, int in_quote)
{
	if (str[i] != '~')
		return (0);
	if (in_quote != 0)
		return (0);
	if (i != 0 && str[i - 1] != ' ')
		return (0);
	if (str[i + 1] && str[i + 1] != ' ' && str[i + 1] != '/')
		return (0);
	return (1);
}

char	*process_expansion(char *str, int *i, t_shell *shell, int in_quote)
{
	char	*temp;

	if (str[*i] == '\\' && in_quote != 1 && str[*i + 1])
	{
		(*i)++;
		temp = ft_substr(str, *i, 1);
		(*i)++;
	}
	else if (should_expand_tilde(str, *i, in_quote))
	{
		temp = expand_tilde(shell);
		(*i)++;
	}
	else if (str[*i] == '$' && in_quote != 1 && str[*i + 1])
		temp = expand_dollar(str, i, shell, in_quote);
	else
	{
		temp = ft_substr(str, *i, 1);
		(*i)++;
	}
	return (temp);
}
