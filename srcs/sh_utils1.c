/*
* Copyright (c) 2020 Arsene Temfack
*
* SPDX-License-Identifier: MIT
*/

#include "minishell.h"

void		prompt(int mode)
{
	if (mode == 0)
		write(STDERR_FILENO, "\x1B[32mMinishell_> \x1B[0m", 21);
}

void		prompt2(void)
{
	signal(SIGINT, sigquit_ctrl_slash_handler);
	write(STDERR_FILENO, "\x1B[34m> \x1B[0m", 11);
}

int			sh_isbackslash(char c)
{
	return (c == '\\' || c == BACKSLASH);
}

int			sh_isquotation(char c)
{
	return (c == '\'' || c == '"' || c == QUOTATION || c == OLDQUOTES);
}

int			sh_is_back_escape(char *line, int i)
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
