/******************************************************************************/
/*     Copyright (c) 2020 Arsene Temfack                                      */
/*                                                                            */
/*     SPDX-License-Identifier: MIT                                           */
/******************************************************************************/

#include "minishell.h"

static void	sh_run(t_data *data, int (*run)(t_data*, int))
{
	t_list	*save_redirections_files[2];

	save_redirections_files[0] = data->cmd[0]->redirections;
	save_redirections_files[1] = data->cmd[0]->files;
	sh_recursive_redirection(data, 0, run);
	data->cmd[0]->redirections = save_redirections_files[0];
	data->cmd[0]->files = save_redirections_files[1];
	if (sh_restore_std_fileno(data->std_fileno) == -1)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
}

int			sh_run_if_non_binary(t_data *data, int n)
{
	if (!ft_strcmp(data->cmd[0]->app, "cd"))
		sh_run(data, sh_cd);
	else if (!ft_strcmp(data->cmd[0]->app, "export"))
		sh_run(data, sh_export);
	else if (!ft_strcmp(data->cmd[0]->app, "unset"))
		sh_run(data, sh_unset);
	else if (!ft_strcmp(data->cmd[0]->app, "exit"))
		sh_run(data, sh_exit);
	else if (!ft_strcmp(data->cmd[0]->app, "pwd"))
		sh_run(data, sh_pwd);
	else
		return (0);
	if (!data->line1[n])
		prompt(data->mode);
	return (1);
}

void		sh_execute_if_non_binary(t_data *data, int n)
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
