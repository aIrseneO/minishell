/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_run.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 09:05:50 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/14 21:31:03 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sh_exit(t_data data, int exno)
{
	sh_free_t_data(&data);
	exit(exno);
}

static int	sh_cd(t_data *data)
{
	int				i;

	if (!data->cmd[0]->argv[1])
	{
		i = 0;
		while (data->envp[i] && ft_strncmp(data->envp[i], "HOME=", 5))
			i++;
		if (!data->envp[i])
			return (sh_perror_return("\x1B[33mMinishell: \x1B[0m",
					"cd", "HOME not set", 1));
		if (chdir(data->envp[i] + 5) == -1)
			return (sh_perror_return("\x1B[33mMinishell: \x1B[0m",
					"cd", strerror(errno), 1));
	}
	else if (data->cmd[0]->argv[2])
		return (sh_perror_return("\x1B[33mMinishell: \x1B[0m", "cd",
				"too many arguments", 1));
	else if (chdir(data->cmd[0]->argv[1]) == -1)
		return (sh_perror_return("\x1B[33mMinishell: cd: \x1B[0m",
				data->cmd[0]->argv[1], strerror(errno), 1));
	return (0);
}

int	sh_run_if_father_app(t_data *data, int n)
{
	if (!ft_strcmp(data->cmd[0]->app, "cd"))
		data->status = sh_cd(data);
	else if (!ft_strcmp(data->cmd[0]->app, "export"))
		data->status = sh_export(data);
	else if (!ft_strcmp(data->cmd[0]->app, "unset"))
		data->status = sh_unset(data);
	else if (!ft_strcmp(data->cmd[0]->app, "exit"))
		sh_exit(*data, 0);
	else
		return (0);
	if (!data->line1[n])
		PROMPT;
	return (1);
}

// watch up for this commands: unset HOME; unset PWD
