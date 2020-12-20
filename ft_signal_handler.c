/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 18:42:48 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/19 22:51:26 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execvp(char *line)
{
	char *ag[] = {"/bin/sh", "-c", line, NULL};
	if (!ft_strcmp(line, "exit"))
		exit(1);
	execvp(ag[0], ag);
	write(1, "Oops, something went wrong!\n", 28);
}

void		sigint_ctrl_c_handler(int signum)
{
	int		wstatus;

	(void)signum;
	if (wait(&wstatus) == -1)
		write(1, "\n\x1B[32mMinishell $> \x1B[0m", 23);
	else if (WIFSIGNALED(wstatus))
	{
		////////////////////////cleanup
		write(1, "\n", 1);
	}
}

void		sigquit_ctrl_slash_handler(int signum)
{
	int		wstatus;

	(void)signum;
	if (wait(&wstatus) != -1 && WIFSIGNALED(wstatus) && WCOREDUMP(wstatus))
		write(1, "Quit (core dumped)\n", 20);
}

