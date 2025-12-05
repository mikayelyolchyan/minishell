/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_empty.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 00:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/05 00:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"
#include "../../include/builtins/builtins.h"

int	handle_empty_command(t_shell *shell)
{
	ft_putstr_fd("minishell: : command not found\n", 2);
	shell->last_exit_status = 127;
	return (127);
}

int	process_empty_args(t_ast_node *cmd_node, t_shell *shell)
{
	shift_empty_args(cmd_node->command->argument);
	if (!cmd_node->command->argument[0])
		return (execute_empty_cmd(cmd_node, shell));
	if (cmd_node->command->argument[0][0] == 1)
		return (handle_empty_command(shell));
	return (-1);
}

void	shift_empty_args(char **args)
{
	int	i;
	int	j;

	if (!args || !args[0])
		return ;
	i = 0;
	while (i < 100 && args[i] && args[i][0] == '\0')
	{
		free(args[i]);
		i++;
	}
	if (i == 0)
		return ;
	j = 0;
	while (args[i])
	{
		args[j] = args[i];
		args[i] = NULL;
		j++;
		i++;
	}
	args[j] = NULL;
}

void	clean_empty_markers(char **args)
{
	int	i;
	int	j;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		if (args[i][0] == 1 && args[i][1] == '\0')
		{
			j = 0;
			while (args[i][j + 1])
			{
				args[i][j] = args[i][j + 1];
				j++;
			}
			args[i][j] = '\0';
		}
		i++;
	}
}
