/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_free_exit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 18:17:44 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/19 23:06:41 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_perror(char *_errmsg, t_cmd *cmds)
{
	if (_errmsg)
	{
		write(1, RED, 5);
		write(1, _errmsg, ft_strlen(_errmsg));
		write(1, RESET, 4);
	}
	if (cmds)
		ft_free_t_cmd(cmds);
	return (errno);
}

void		ft_free_and_exit(t_cmd *cmds)
{
	ft_free_t_cmd(cmds);
	exit(errno);
}

void		ft_free_t_cmd(t_cmd *cmds)
{
	(void)cmds;
}
