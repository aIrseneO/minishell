/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_syntax_check_utils3.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 20:52:36 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/14 20:53:24 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sh_memcpy(char *dest, char *src, int n)
{
	while (n--)
	{
		if (*src == '\\')
		{
			if (*(src + 1) == '\\')
			{
				src++;
				*dest++ = *src++;
				*dest++ = BACKSLASH;
				n--;
			}
			else
			{
				src++;
				*dest++ = BACKSLASH;
			}
		}
		else if (*src == '$' && ft_isquotation(*(src + 1)))
		{
			src++;
			*dest++ = NONDOLLAR;
		}
		else
			*dest++ = *src++;
	}
}
