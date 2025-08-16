/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:43 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/16 17:27:17 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "minishell.h"
#include "../include/prompt/prompt.h"
#include "../lib/libft/libft.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	(void)envp;
	if (argc != 1)
	{
		ft_putstr_fd("?\n", 2);
		return (1);
	}
	get_prompt_line();
	return (0);
}
