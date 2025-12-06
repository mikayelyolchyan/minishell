/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins/builtins.h"
#include <limits.h>

static int	check_overflow(char *str, int sign)
{
	unsigned long long	result;
	int					i;

	result = 0;
	i = 0;
	while (str[i])
	{
		if (result > LLONG_MAX / 10)
			return (1);
		result = result * 10 + (str[i] - '0');
		if (sign == 1 && result > LLONG_MAX)
			return (1);
		if (sign == -1 && result > (unsigned long long)LLONG_MAX + 1)
			return (1);
		i++;
	}
	return (0);
}

static int	is_numeric(char *str)
{
	int	i;
	int	sign;

	i = 0;
	sign = 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	if (str[0] == '+' || str[0] == '-')
		return (!check_overflow(str + 1, sign));
	return (!check_overflow(str, sign));
}

int	builtin_exit(char **args, t_shell *shell)
{
	int	exit_code;

	exit_code = 0;
	if (!shell->in_subshell)
		ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args[1])
		exit_code = shell->last_exit_status;
	else if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		exit_code = 2;
	}
	else if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	else
		exit_code = ft_atoi(args[1]);
	shell->last_exit_status = exit_code;
	shell->should_exit = 1;
	return (exit_code);
}
