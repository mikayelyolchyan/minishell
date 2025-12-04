/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:43 by miyolchy          #+#    #+#             */
/*   Updated: 2025/11/30 17:58:58 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/env/env.h"
#include "../include/prompt/prompt.h"
#include "../include/signals/signals.h"
#include "../lib/libft/libft.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	t_env	*env_list;

	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("?\n", 2);
		return (1);
	}
	env_list = get_env_list(envp);
	init_shell(&shell, env_list);
	setup_signals_interactive();
	get_prompt_line(&shell);
	free_env_list(shell.env_list);
	return (shell.last_exit_status);
}
