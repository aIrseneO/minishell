/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 12:21:22 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/20 02:22:03 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_init(t_cmd *cmds, char **env)
{
	cmds->envp = env;////////////////
	ft_init_t_cmd(cmds);
	return (1);
}

void			ft_init_t_cmd(t_cmd *cmds)
{
	cmds->do_fork = 1;
	char *s = "hey";

	cmds->line1 = (char **)malloc(sizeof(*cmds->line1) * 2);
	cmds->line1[0] = s;
	cmds->line1[1] = NULL;
}
