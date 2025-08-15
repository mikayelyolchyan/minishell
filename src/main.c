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
