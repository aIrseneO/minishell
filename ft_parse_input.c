/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 20:53:17 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/26 00:31:35 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_parse_input(t_cmd *cmds)
{
	int		n;

	cmds->line1 = (char **)malloc(sizeof(char *) * 2);
	cmds->line1[0] = ft_strdup("mouf\n");
	cmds->line1[1] = NULL;
	(void)cmds;
	return (1);
	if ((cmds->line1 = ft_split(cmds->line, ';')) == NULL)
		return (-1);
	n = 0;
	while (cmds->line1[n])
		n++;
	
}
