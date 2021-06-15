/******************************************************************************/
/*     Copyright (c) 2020 Arsene Temfack                                      */
/*                                                                            */
/*     SPDX-License-Identifier: MIT                                           */
/******************************************************************************/

#include "minishell.h"

void		sig_handler1(int signum)
{
	(void)signum;
}

void		sig_handler2(int signum)
{
	write(STDERR_FILENO, "\n", 1);
	(void)signum;
}
