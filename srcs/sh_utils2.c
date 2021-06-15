/******************************************************************************/
/*     Copyright (c) 2020 Arsene Temfack                                      */
/*                                                                            */
/*     SPDX-License-Identifier: MIT                                           */
/******************************************************************************/

#include "minishell.h"

void	sh_get_command_line(t_data *data, int n)
{
	while (sh_syntax_check(data) == -1)
	{
		n = get_next_line(data->fd, &data->line);
		if (n == -1)
			sh_free_data_exit1(data, NULL, strerror(errno), X);
		if (n == 0)
		{
			if (!(*data->line))
				sigexit_ctrl_d_handler(data, data->status);
			sigappend_ctrl_d_handler(data, &data->line, n);
		}
	}
}

int		sh_save_std_fileno(int std_fileno[3])
{
	std_fileno[0] = dup(STDOUT_FILENO);
	std_fileno[1] = dup(STDIN_FILENO);
	std_fileno[2] = dup(STDERR_FILENO);
	if (std_fileno[0] == -1 || std_fileno[1] == -1 || std_fileno[2] == -1)
		return (-1);
	return (0);
}

int		sh_restore_std_fileno(int std_fileno[3])
{
	if (dup2(std_fileno[0], STDOUT_FILENO) == -1 ||
			dup2(std_fileno[1], STDIN_FILENO) == -1 ||
			dup2(std_fileno[2], STDERR_FILENO) == -1)
		return (-1);
	return (0);
}

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
