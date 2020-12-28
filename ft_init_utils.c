/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 00:27:43 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/28 00:38:38 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_init_scmd(t_cmd **scmd, int n)
{
	if ((*scmd = (t_cmd *)malloc(sizeof(**scmd) * n)) == NULL)
		return (-1);
	while (--n >= 0)
	{
		(*scmd)[n].app = NULL;
		(*scmd)[n].args = NULL;
		(*scmd)[n].redirections = NULL;
		(*scmd)[n].files = NULL;
	}
	return (1);
}
