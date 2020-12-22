/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 18:17:44 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/21 22:24:40 by atemfack         ###   ########.fr       */
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
	free(cmds->line);
	ft_strfree(&cmds->envp);
	ft_strfree(&cmds->line1);
	ft_strfree(&cmds->line2);
	//ft_strfree(cmds->);
	//ft_strfree(cmds->);
}
