/******************************************************************************/
/*     Copyright (c) 2020 Arsene Temfack                                      */
/*                                                                            */
/*     SPDX-License-Identifier: MIT                                           */
/******************************************************************************/

/*
** gw echo.c libft.a -o bin/echo
*/

#include "libft.h"

static int	ft_isoption(char *av, int *endl)
{
	if (!ft_strncmp(av, "-n", 2))
	{
		av += 2;
		while (*av && *av == 'n')
			av++;
		if (*av)
			return (0);
		*endl = 1;
		return (1);
	}
	return (0);
}

int			main(int argc, char **argv)
{
	char	**av;
	int		endl;

	if (argc == 1)
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	av = argv + 1;
	endl = 0;
	while (ft_isoption(*av, &endl))
		av++;
	if (!(*av))
		return (0);
	while (*av && *(av + 1))
	{
		ft_putstr_fd(*av++, 1);
		ft_putchar_fd(' ', 1);
	}
	if (!endl)
		ft_putendl_fd(*av, 1);
	else
		ft_putstr_fd(*av, 1);
	return (0);
}
