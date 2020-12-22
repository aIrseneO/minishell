/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/13 19:12:20 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/20 17:34:15 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int 		main(int ac, char **av, char **env)
{
	int		endl;

	if (ac == 1)
	{
		write(1, "\n", 2);
		return (0);
	}
	endl = ft_strcmp(av[1], "-n") ? 1 :++ 0;
	if (!ft_strcmp(av[1], "-n") && ac == 2)
		return (0);
	
	ft_printf("%s", echo(av, env));
	return (0);
}
