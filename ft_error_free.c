/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 18:17:44 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/26 00:29:11 by atemfack         ###   ########.fr       */
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

void		ft_free_t_cmd(t_cmd *cmds)
{
	ft_envpclear(&cmds->envp, ft_astrfree);
	free(cmds->line);
	ft_astrfree(&cmds->line1);
	ft_astrfree(&cmds->line2);
	//ft_astrfree(cmds->);
	//ft_astrfree(cmds->);
}
