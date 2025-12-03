/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 22:45:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/03 20:25:50 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expansion/expansion.h"
#include "../../include/env/env.h"

static char	*get_var_value(char *var_name, t_shell *shell)
{
	t_env	*env;

	if (!var_name)
		return (NULL);
	if (var_name[0] == '?' && var_name[1] == '\0')
		return (ft_itoa(shell->last_exit_status));
	env = shell->env_list;
	while (env)
	{
		if (ft_strncmp(env->name, var_name, ft_strlen(var_name)) == 0
			&& env->name[ft_strlen(var_name)] == '\0')
		{
			if (env->value)
				return (ft_strdup(env->value));
			return (ft_strdup(""));
		}
		env = env->next;
	}
	return (ft_strdup(""));
}

static int	get_var_name_len(char *str)
{
	int	len;

	len = 0;
	if (str[0] == '?')
		return (1);
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

char	*expand_dollar(char *str, int *i, t_shell *shell, int in_quote)
{
	char	*var_name;
	char	*var_value;
	int		name_len;

	if (in_quote == 1)
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	(*i)++;
	name_len = get_var_name_len(&str[*i]);
	if (name_len == 0)
	{
		if (str[*i] && (str[*i] == '"' || str[*i] == '\'') && in_quote == 0)
			return (ft_strdup(""));
		return (ft_strdup("$"));
	}
	var_name = ft_substr(str, *i, name_len);
	if (!var_name)
		return (NULL);
	var_value = get_var_value(var_name, shell);
	free(var_name);
	*i += name_len;
	return (var_value);
}

static void	handle_quotes(char c, int *in_quote)
{
	if (c == '\'' && *in_quote != 2)
	{
		if (*in_quote == 1)
			*in_quote = 0;
		else
			*in_quote = 1;
	}
	else if (c == '"' && *in_quote != 1)
	{
		if (*in_quote == 2)
			*in_quote = 0;
		else
			*in_quote = 2;
	}
}

char	*expand_variables(char *str, t_shell *shell)
{
	char	*result;
	char	*temp;
	int		i;
	int		in_quote;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	in_quote = 0;
	while (str[i])
	{
		handle_quotes(str[i], &in_quote);
		temp = process_expansion(str, &i, shell, in_quote);
		result = join_and_free(result, temp);
	}
	return (result);
}
