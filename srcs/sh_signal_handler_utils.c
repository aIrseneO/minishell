/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_signal_handler_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 18:42:48 by atemfack          #+#    #+#             */
/*   Updated: 2021/03/11 23:43:59 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
