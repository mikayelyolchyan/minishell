/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cleanup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/06 00:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env/env.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

void	cleanup_shell(t_shell *shell)
{
	int	i;

	if (shell->env_list)
	{
		free_env_list(shell->env_list);
		shell->env_list = NULL;
	}
	if (shell->env_array)
	{
		i = 0;
		while (shell->env_array[i])
			free(shell->env_array[i++]);
		free(shell->env_array);
		shell->env_array = NULL;
	}
	if (shell->current_line)
	{
		free(shell->current_line);
		shell->current_line = NULL;
	}
	rl_clear_history();
}
