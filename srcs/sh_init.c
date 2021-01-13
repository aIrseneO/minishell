/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 12:21:22 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/13 00:01:32 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sh_reset_t_data(t_data *data)
{
	free(data->line);
	data->line = NULL;
	ft_astrfree(&data->line1, free);
	ft_astrfree(&data->line2, free);
	sh_cmdfree(&data->cmd);
}

int	sh_init_cmd(t_cmd ***cmd, int n)
{
	int			i;

	*cmd = (t_cmd **)malloc(sizeof(**cmd) * (n + 1));
	if (*cmd == NULL)
		return (-1);
	i = 0;
	while (i <= n)
		(*cmd)[i++] = NULL;
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
	return (1);
}

void	sh_zerodata(t_data *data)
{
	data->pwd = NULL;
	data->path = NULL;
	data->envp = NULL;
	data->line = NULL;
	data->line1 = NULL;
	data->line2 = NULL;
	data->cmd = NULL;
	data->status = 0;
}

int	sh_init(t_data *data, char **env)
{
	int			nbr_envp_initial;

	nbr_envp_initial = 4;
	sh_zerodata(data);
	data->envp = ft_astrinit(nbr_envp_initial + 1);
	if (data->envp == NULL)
		return (-1);
	if (sh_load_envp(env, data->envp, &data->pwd) == -1)
	{
		ft_astrnfree(&data->envp, nbr_envp_initial + 1);
		sh_free_t_data(data);
		return (-1);
	}
	data->path = ft_astrinit(2);
	if (data->path == NULL)
		return (-1);
	data->path[0] = ft_strdup(data->envp[3] + 5);
	if (data->path[0] == NULL)
		return (-1);
	return (1);
}
