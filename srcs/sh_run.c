/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_run.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 09:05:50 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/14 13:10:06 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	sh_save(t_data *data, int std_out_in[2], t_list *redirect_file[2])
{
	std_out_in[0] = dup(STDOUT_FILENO);
	std_out_in[1] = dup(STDIN_FILENO);
	if (!std_out_in[0] || !std_out_in[1])
	{
		return (sh_perror_return("\x1B[31mMinishell: \x1B[0m",
			"Couldn't save a STD_FILENO", strerror(errno), -1));
	}
	redirect_file[0] = data->cmd[0]->redirections;
	redirect_file[1] = data->cmd[0]->files;
	return (0);
}

static int	sh_restore(t_data *data, int std_out_in[2], t_list *redrct_file[2])
{
	if (dup2(std_out_in[0], STDOUT_FILENO) == -1
		|| dup2(std_out_in[1], STDIN_FILENO) == -1)
	{
		return (sh_perror_return("\x1B[31mMinishell: \x1B[0m",
			"Couldn't restore a STD_FILENO", strerror(errno), -1));
	}
	data->cmd[0]->redirections = redrct_file[0];
	data->cmd[0]->files = redrct_file[1];
	return (0);
}

int			sh_run_if_father_app(t_data *data, int n)
{
	int		std_out_in[2];
	t_list	*redirections_files[2];

	if (sh_save(data, std_out_in, redirections_files) == -1)
		return (-1);
	if (!ft_strcmp(data->cmd[0]->app, "cd"))
		sh_recursive_redirection(data, 0, sh_cd);
	else if (!ft_strcmp(data->cmd[0]->app, "export"))
		sh_recursive_redirection(data, 0, sh_export);
	else if (!ft_strcmp(data->cmd[0]->app, "unset"))
		sh_recursive_redirection(data, 0, sh_unset);
	else if (!ft_strcmp(data->cmd[0]->app, "exit"))
		sh_recursive_redirection(data, 0, sh_exit);
	else if (!ft_strcmp(data->cmd[0]->app, "pwd"))
		sh_recursive_redirection(data, 0, sh_pwd);
	else
		return (0);
	if (sh_restore(data, std_out_in, redirections_files) == -1)
		return (-1);
	if (!data->line1[n])
		prompt(data->mode);
	return (1);
}

void		sh_execute_if_father_app(t_data *data, int n)
{
	if (!ft_strcmp(data->cmd[n]->app, "cd"))
		exit(sh_cd(data, n));
	else if (!ft_strcmp(data->cmd[n]->app, "export"))
		exit(sh_export(data, n));
	else if (!ft_strcmp(data->cmd[n]->app, "unset"))
		exit(sh_unset(data, n));
	else if (!ft_strcmp(data->cmd[n]->app, "exit"))
		exit(sh_exit(data, n));
	else if (!ft_strcmp(data->cmd[n]->app, "pwd"))
		exit(sh_pwd(data, n));
}
