/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 23:41:29 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/08 01:12:29 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "libft.h"

//gw pwd.c libft.a -o bin/pwd

int	main(void)
{
	char	pwd[4096];

	if (!getcwd(pwd, 4096))
	{
		ft_putstr_fd("pwd: ", 1);
		ft_putstr_fd(strerror(errno), 1);
		return (1);
	}
	ft_putendl_fd(pwd, 1);
	return (0);
}
