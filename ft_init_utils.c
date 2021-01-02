/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 00:27:43 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/29 21:50:41 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_reset_t_data(t_data *data)
{
	free(data->line);
	ft_astrfree(&data->line1);
	ft_astrfree(&data->line2);
	ft_cmdfree(&data->cmd);
	ft_zero_t_data(data);
}

void		ft_zero_t_data(t_data *data)
{
	data->pwd = NULL;
	data->line = NULL;
	data->line1 = NULL;
	data->line2 = NULL;
	data->cmd = NULL;
}

int			ft_init_cmd(t_cmd ***cmd, int n)
{
	int		i;

	if ((*cmd = (t_cmd **)malloc(sizeof(**cmd) * (n + 1))) == NULL)
		return (-1);
	i = 0;
	while (i <= n)
		(*cmd)[i++] = NULL;
	i = 0;
	while (i < n)
	{
		if (((*cmd)[i] = (t_cmd *)malloc(sizeof(***cmd))) == NULL)
			return (-1);
		(*cmd)[i]->app = NULL;
		(*cmd)[i]->args = NULL;
		(*cmd)[i]->redirections = NULL;
		(*cmd)[i]->files = NULL;
		(*cmd)[i++]->argv = NULL;
	}
	return (1);
}
