/*
* Copyright (c) 2020 Arsene Temfack
*
* SPDX-License-Identifier: MIT
*/

#include "libft.h"

/*
** gw env.c libft.a -o bin/env
*/

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	while (*envp)
		ft_putendl_fd(*envp++, 1);
	return (0);
}
