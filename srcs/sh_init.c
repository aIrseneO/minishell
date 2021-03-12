/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 12:21:22 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/15 01:50:50 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		sh_reset_t_data(t_data *data)
{
	ft_strdel(&data->line);
	ft_astrfree(&data->line1, free);
	ft_astrfree(&data->line2, free);
	free(data->children_pid);
	data->children_pid = NULL;
	sh_cmdfree(&data->cmd);
}

int			sh_init_cmd(t_cmd ***cmd, int n)
{
	int		i;

	*cmd = (t_cmd **)malloc(sizeof(**cmd) * (n + 1));
	if (*cmd == NULL)
		return (-1);
	i = 0;
	while (i < n)
	{
		(*cmd)[i] = (t_cmd *)malloc(sizeof(***cmd));
		if ((*cmd)[i] == NULL)
			return (-1);
		(*cmd)[i]->app = NULL;
		(*cmd)[i]->args = NULL;
		(*cmd)[i]->redirections = NULL;
		(*cmd)[i]->files = NULL;
		(*cmd)[i++]->argv = NULL;
	}
	(*cmd)[i] = NULL;
	return (0);
}

static void	sh_get_fd_or_line(t_data *data)
{
	if (data->ac == 1)
		return ;
	if (!ft_strcmp(data->av[1], "-c"))
	{
		data->line = ft_strjoin(data->av[2], ";exit");
		data->mode = 1;
		if (data->line == NULL)
			data->fd = -1;
		return ;
	}
	data->mode = 3;
	data->fd = open(data->av[1], O_RDONLY);
	if (data->fd == -1)
	{
		sh_free_t_data(data);
		sh_free_data_exit2(data, data->av[1], strerror(errno), 126);
	}
}

/*
** Set Shellpid: data->shellpid = getpid();
*/

static void	sh_zerodata_and_init_const(t_data *data, int ac, char **av)
{
	data->ac = ac;
	data->av = av;
	data->fd = STDIN_FILENO;
	data->mode = 0;
	if (sh_save_std_fileno(data->std_fileno) == -1)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
	data->pwd = NULL;
	data->pwd_env = NULL;
	data->ispwd_env = 1;
	data->oldpwd_env = NULL;
	data->isoldpwd_env = 1;
	data->path = NULL;
	data->envp = NULL;
	data->envpl = (t_list **)malloc(sizeof(*data->envpl));
	data->line = NULL;
	data->line1 = NULL;
	data->ispiped = 0;
	data->line2 = NULL;
	data->children_pid = NULL;
	data->cmd = NULL;
	data->status = 0;
}

void		sh_init(t_data *data, int ac, char **av, char **env)
{
	sh_zerodata_and_init_const(data, ac, av);
	if (data->envpl == NULL)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
	*data->envpl = NULL;
	sh_get_fd_or_line(data);
	if (data->fd == -1)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
	data->envp = ft_astrdup(env);
	if (!(*env) && data->envp == NULL)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
	if (ft_astrtolst(data->envpl, data->envp) == -1)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
	data->pwd = ft_getcwd();
	if (data->pwd == NULL)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
	sh_update_envp(data, 0);
}
