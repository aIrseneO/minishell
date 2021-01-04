/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 12:21:22 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/03 23:38:27 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_reset_t_data(t_data *data)
{
	free(data->line);
	data->line = NULL;
	ft_astrfree(&data->line1);
	ft_astrfree(&data->line2);
	ft_cmdfree(&data->cmd);
}

int				ft_init_cmd(t_cmd ***cmd, int n)
{
	int			i;

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

static int		ft_load_path_pwd(char **envp, char **pwd)
{
	int			ret;
	char		*tmp;

	if (!(*pwd = ft_getcwd()))
		return (-1);
	tmp = NULL;
	ret = 1;
	if (!(envp[3] = ft_strjoin("PWD=", *pwd)) ||
			!(tmp = ft_strjoin("PATH=", *pwd)) ||
			!(envp[4] = ft_strjoin(tmp, "/bin")))
		ret = -1;
	free(tmp);
	return (ret);
}

static int		ft_load_envp(char **env, char **envp, char **pwd)
{
	while (*env)
	{
 		if ((!ft_strncmp(*env, "USER=", 5) && !(envp[1] = ft_strdup(*env))) ||
			(!ft_strncmp(*env, "HOME=", 5) && !(envp[2] = ft_strdup(*env))))
						return (-1);
		env++;
	}
	if (!(envp[0] = ft_strdup("?=0")) ||
			(!envp[1] && !(envp[1] = ft_strdup(USER))) ||
			(!envp[2] && !(envp[2] = ft_strdup(HOME))))
		return (-1);
	return (ft_load_path_pwd(envp, pwd));
}

int				ft_init(t_data *data, char **env)
{
	int			nbr_envp_initial;

	nbr_envp_initial = 5;
	data->pwd = NULL;
	data->path = NULL;
	data->envp = NULL;
	data->line = NULL;
	data->line1 = NULL;
	data->line2 = NULL;
	data->cmd = NULL;
	if ((data->envp = ft_astrinit(nbr_envp_initial + 1)) == NULL)
		return (-1);
	if (ft_load_envp(env, data->envp, &data->pwd) == -1)
	{
		ft_astrnfree(&data->envp, nbr_envp_initial + 1);
		ft_free_t_data(data);
		return (-1);
	}
	if (!(data->path = ft_astrinit(2)) ||
			!(data->path[0] = ft_strdup(data->envp[4] + 5)))
		return (-1);
	return (1);
}
