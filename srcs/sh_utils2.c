/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 04:35:29 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/13 04:37:06 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** For debugging only
*/

void	sh_putstr_fd(char *s, int fd)
{
	write(fd, "*>>", 3);
	while (*s)
	{
		if (*s == QUOTATION)
			write(fd, "\x1B[37m\"\x1B[0m", 10);
		else if (*s == BACKSLASH)
			write(fd, "\x1B[35m\\\x1B[0m", 10);
		else if (*s == DOLLARSIG)
			write(fd, "\x1B[34m$\x1B[0m", 10);
		else if (*s == NONDOLLAR)
			write(fd, "\x1B[35m$\x1B[0m", 10);
		else if (*s == OLDQUOTES)
			write(fd, "\x1B[35m'\x1B[0m", 10);
		else
			ft_putchar_fd(*s, fd);
		s++;
	}
	write(fd, "<<*\n", 4);
}
