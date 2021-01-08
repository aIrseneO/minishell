/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_father_app.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 09:05:50 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/07 21:01:57 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void				ft_exit(t_data data)
{
	ft_free_t_data(&data);
	exit(0);
}

static int			ft_cd(t_data *data)
{
	int				i;

	if (!data->cmd[0]->argv[1])
	{
		i = 0;
		while (data->envp[i] && ft_strncmp(data->envp[i], "HOME=", 5))
			i++;
		if (!data->envp[i])
			return (ft_perror_return("\x1B[33mMinishell: \x1B[0m",
					"cd", "HOME not set", 1));
		if (chdir(data->envp[i] + 5) == -1)
			return (ft_perror_return("\x1B[33mMinishell: \x1B[0m",
					"cd", strerror(errno), 1));
	}
	else if (data->cmd[0]->argv[2])
		return (ft_perror_return("\x1B[33mMinishell: \x1B[0m", "cd",
				"too many arguments", 1));
	else if (chdir(data->cmd[0]->argv[1]) == -1)
		return (ft_perror_return("\x1B[33mMinishell: \x1B[0m",
				"cd", strerror(errno), 1));
	return (0);
}

int					ft_execute_if_father_app(t_data *data)
{
	if (!ft_strcmp(data->cmd[0]->app, "cd"))
		data->status = ft_cd(data);
	else if (!ft_strcmp(data->cmd[0]->app, "export"))
		data->status = ft_export(data);
	else if (!ft_strcmp(data->cmd[0]->app, "unset"))
		data->status = ft_unset(data);
	else if (!ft_strcmp(data->cmd[0]->app, "exit"))
		ft_exit(*data);
	else
		return (0);
	return (1);
}

// watch up for this commands: unset HOME; unset PWD
