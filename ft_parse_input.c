/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 20:53:17 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/26 19:03:34 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_parse_cmd(t_cmd *cmds, int i)
{
	if ((cmds->line2 = ft_split(cmds->line1[i], '|')) == NULL)
		return (-1);

	//ft_printf("--->%s\n", cmds->line1[i]);

	cmds->do_fork = 1;

	return (1);
}
