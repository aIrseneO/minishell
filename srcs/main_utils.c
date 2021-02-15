/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 00:51:01 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/15 03:55:05 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sh_get_line(t_data *data, int n)
{
	char	*tmp1;
	char	*tmp2;

	n = get_next_line(data->fd, &data->line);
	if (!(*data->line) && n == 0)
		sigexit_ctrl_d_handler(data);
	while (n == 0 && !data->mode)
	{
		tmp1 = data->line;
		n = get_next_line(data->fd, &data->line);
		if (n == -1)
		{
			free(tmp1);
			exit(sh_perror_free_t_data(strerror(errno), data));
		}
		tmp2 = data->line;
		data->line = ft_strjoin(tmp1, data->line);
		free(tmp1);
		free(tmp2);
		if (data->line == NULL)
			exit(sh_perror_free_t_data(strerror(errno), data));
	}
	return (0);
}
