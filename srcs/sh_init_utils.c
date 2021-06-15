/******************************************************************************/
/*     Copyright (c) 2020 Arsene Temfack                                      */
/*                                                                            */
/*     SPDX-License-Identifier: MIT                                           */
/******************************************************************************/

#include "minishell.h"

static int	sh_export_new(t_data *data, char *str, int n)
{
	if (str == NULL)
		return (-1);
	n = sh_export_str(data, str, n);
	free(str);
	return (n);
}

static int	sh_export_(t_data *data, char *str)
{
	int		n;
	char	*tmp;

	n = ft_atoi(str) + 1;
	tmp = ft_itoa(n);
	if (tmp == NULL)
		return (-1);
	n = sh_export_new(data, ft_strjoin("SHLVL=", tmp), 5);
	free(tmp);
	return (n);
}

static int	sh_update(t_data *data, int is_env[3])
{
	char	*path;

	path = PATH_ENV;
	if (!is_env[0]
		&& sh_export_new(data, ft_strjoin("PWD=", data->pwd), 3) == -1)
		return (-1);
	if (is_env[1] == -1 && sh_export_new(data, ft_strdup("SHLVL=1"), 5) == -1)
		return (-1);
	else if (is_env[1] > -1 && sh_export_(data,
			data->envp[is_env[1]] + 6) == -1)
		return (-1);
	if (is_env[2] == -1 && path[5] != '_')
		data->path = ft_split(path + 5, ':');
	else if (is_env[2] > -1)
	{
		if (path[5] != '_' && sh_export_new(data,
				ft_strjoin2(path, ":", data->envp[is_env[2]] + 5), 4) == -1)
			return (-1);
		data->path = ft_split(data->envp[is_env[2]] + 5, ':');
	}
	if (data->path == NULL)
		return (-1);
	return (0);
}

void		sh_update_envp(t_data *data, int i)
{
	int		is_env[3];

	is_env[0] = 0;
	is_env[1] = -1;
	is_env[2] = -1;
	while (data->envp[i])
	{
		if (!ft_strncmp(data->envp[i], "PWD=", 4))
			is_env[0] = 1;
		if (!ft_strncmp(data->envp[i], "SHLVL=", 6))
			is_env[1] = i;
		if (!ft_strncmp(data->envp[i], "PATH=", 5))
			is_env[2] = i;
		i++;
	}
	if (sh_update(data, is_env) == -1)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
}
