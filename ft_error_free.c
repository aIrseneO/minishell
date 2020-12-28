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

int			ft_perror(char *_errmsg, t_data *data)
{
	if (_errmsg)
	{
		write(1, RED, 5);
		write(1, _errmsg, ft_strlen(_errmsg));
		write(1, RESET, 4);
	}
	if (data)
		ft_free_t_data(data);
	return (errno);
}

void		ft_free_t_data(t_data *data)
{
	ft_envpclear(&data->envp, ft_astrfree);
	free(data->line);
	ft_astrfree(&data->line1);
	ft_astrfree(&data->line2);
	//ft_astrfree(data->);
	//ft_astrfree(data->);
}
