/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:43 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/15 18:01:44 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv)
{
	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("?\n", 2);
		return (1);
	}
	ft_putstr_fd("Makefile\n", 1);
	return (0);
}
