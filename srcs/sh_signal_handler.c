/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_signal_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 18:42:48 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/15 01:39:50 by atemfack         ###   ########.fr       */
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

void	sigexit_ctrl_d_handler(t_data *data)
{
	int	exno;

	if (!data->mode)
		write(STDERR_FILENO, "exit\n", 5);
	exno = data->status;
	sh_free_t_data(data);
	exit(exno);
}
