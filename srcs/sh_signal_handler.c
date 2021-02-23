/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_signal_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 18:42:48 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/23 04:15:53 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_ctrl_c_handler(int signum)
{
	write(STDERR_FILENO, "\n", 1);
	prompt(0);
	(void)signum;
}

void	sigquit_ctrl_slash_handler(int signum)
{
	(void)signum;
}

void	sigexit_ctrl_d_handler(t_data *data, int exno)
{
	if (!data->mode)
		write(STDERR_FILENO, "exit\n", 5);
	sh_free_t_data(data);
	exit(exno);
}

// When ctrl+d is catch, this function append new input to old one.

void	sigappend_ctrl_d_handler(t_data *data, char **line, int n)
{
	char	*tmp1;
	char	*tmp2;

	while (n == 0 && !data->mode)
	{
		tmp1 = *line;
		n = get_next_line(data->fd, line);
		if (n == -1)
		{
			free(tmp1);
			exit(sh_perror_free_t_data(strerror(errno), data));
		}
		tmp2 = *line;
		*line = ft_strjoin(tmp1, *line);
		free(tmp1);
		free(tmp2);
		if (*line == NULL)
			exit(sh_perror_free_t_data(strerror(errno), data));
	}
}
