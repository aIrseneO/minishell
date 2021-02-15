/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 04:33:32 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/15 01:47:09 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prompt(int mode)
{
	if (mode == 0)
		write(STDERR_FILENO, "\n\x1B[32mMinishell_> \x1B[0m", 22);
}

int	sh_isbackslash(char c)
{
	return (c == '\\' || c == BACKSLASH);
}

int	sh_isquotation(char c)
{
	return (c == '\'' || c == '"' || c == QUOTATION || c == OLDQUOTES);
}

int	sh_is_back_escape(char *line, int i)
{
	int		n;

	n = 0;
	while (i >= 0 && sh_isbackslash(line[i]))
	{
		n++;
		i--;
	}
	return (n % 2);
}
