/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_signal_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 18:42:48 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/12 03:08:32 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_ctrl_c_handler(int signum)
{
	write(1, "\n", 1);
	PROMPT;
	(void)signum;
}

void	sigquit_ctrl_slash_handler(int signum)
{
	(void)signum;
}

void	sigexit_ctrl_d_handler(t_data *data)
{
	sh_free_t_data(data);
	write(1, "\n", 1);
	exit(0);
}
