/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins/builtins.h"

static void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

static int	validate_and_update(t_shell *shell, char *name,
	char *value, char *equals)
{
	if (!is_valid_identifier(name))
	{
		if (equals)
			free(name);
		return (1);
	}
	update_or_add_env_var(shell, name, value);
	if (equals)
		free(name);
	return (0);
}

static int	process_export_arg(t_shell *shell, char *arg)
{
	char	*equals;
	char	*name;
	char	*value;
	int		result;

	equals = find_equals_sign(arg);
	if (!equals)
	{
		name = arg;
		value = NULL;
	}
	else
	{
		name = ft_substr(arg, 0, equals - arg);
		value = equals + 1;
	}
	result = validate_and_update(shell, name, value, equals);
	if (result != 0)
		print_export_error(arg);
	return (result);
}

int	builtin_export(char **args, t_shell *shell)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_all_exports(shell->env_list);
		return (0);
	}
	i = 1;
	ret = 0;
	while (args[i])
	{
		if (process_export_arg(shell, args[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
