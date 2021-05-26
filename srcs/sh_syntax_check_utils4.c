/*
* Copyright (c) 2020 Arsene Temfack
*
* SPDX-License-Identifier: MIT
*/

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
