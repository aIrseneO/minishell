/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 13:31:33 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/13 23:19:28 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sh_put_free(t_data *data, int i)
{
	ft_putstr_fd("\x1B[33mMinishell: \x1B[0mexit: ", STDERR_FILENO);
	ft_putstr_fd(data->cmd[i]->argv[1], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	sh_free_t_data(data);
}

int			sh_exit(t_data *data, int i)
{
	int		exno;

	if (data->mode == 0)
		write(STDERR_FILENO, "exit\n", 5);
	if (!data->cmd[i]->argv[1])
	{
		exno = data->status;
		sh_free_t_data(data);
		exit((unsigned char)exno);
	}
	if (!ft_isnumber(data->cmd[i]->argv[1]))
	{
		sh_put_free(data, i);
		exit(2);
	}
	if (data->cmd[i]->argv[2])
	{
		return (sh_perror_return("\x1B[33mMinishell: \x1B[0m", "exit",
				"too many arguments", 1));
	}
	exno = ft_atoi(data->cmd[i]->argv[1]);
	sh_free_t_data(data);
	exit((unsigned char)exno);
	return (sh_perror_return("Oops something went wrong!", NULL, ": exit", -1));
}
