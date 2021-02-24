/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 23:41:15 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/08 01:36:08 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
