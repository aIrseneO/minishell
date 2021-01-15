/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 23:40:49 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/14 22:02:17 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//gw echo.c libft.a -o bin/echo

#include "libft.h"

int	main(int ac, char **av)
{
	int		endl;

	if (ac == 1)
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	endl = 0;
	if (!ft_strcmp(*(++av), "-n"))
	{
		av++;
		endl = 1;
	}
	if (endl && ac == 2)
		return (0);
	while (*(av + 1))
	{
		ft_putstr_fd(*av++, 1);
		ft_putchar_fd(' ', 1);
	}
	if (endl)
		ft_putendl_fd(*av, 1);
	else
		ft_putstr_fd(*av, 1);
	return (0);
}
