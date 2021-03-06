/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 23:41:29 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/28 22:44:08 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

int	main(int ac, char **av)
{
	char	pwd[4096];

	(void)ac;
	if (!getcwd(pwd, 4096))
	{
		ft_putstr_fd(av[0], 1);
		ft_putendl_fd(
			": couldn't find directory entry in ‘..’ with matching i-node", 1);
		return (1);
	}
	ft_putendl_fd(pwd, 1);
	return (0);
}
