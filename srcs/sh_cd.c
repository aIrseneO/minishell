/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 20:28:23 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/12 22:03:02 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	sh_update_env_wd(t_data *data)
{
	char	*wd;

	if (data->ispwd_env)
	{
		wd = ft_strjoin("PWD=", data->pwd_env);
		if (wd == NULL)
			return (-1);
		if (sh_export_str(data, wd, 3))
			return (-1);
		free(wd);
	}
	if (data->isoldpwd_env && data->oldpwd_env)
	{
		wd = ft_strjoin("OLDPWD=", data->oldpwd_env);
		if (wd == NULL)
			return (-1);
		if (sh_export_str(data, wd, 6))
			return (-1);
		free(wd);
	}
	return (0);
}

static int	sh_update_env_var(t_data *data, char *dir)
{
	free(data->pwd);
	data->pwd = ft_strdup(dir);
	if (data->pwd == NULL)
	{
		return (sh_perror_return("\x1B[31mMinishell: \x1B[0m",
			"cd", strerror(errno), -1));
	}
	free(data->oldpwd_env);
	data->oldpwd_env = data->pwd_env;
	data->pwd_env = ft_strdup(dir);
	if (data->pwd_env == NULL)
	{
		return (sh_perror_return("\x1B[31mMinishell: \x1B[0m",
			"cd", strerror(errno), -1));
	}
	if (sh_update_env_wd(data) == -1)
	{
		return (sh_perror_return("\x1B[31mMinishell: \x1B[0m",
			"cd", strerror(errno), -1));
	}
	return (0);
}

static int	sh_update(t_data *data, char *dir, int ishyphen)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		sh_perror_return("cd: error retrieving current directory: ",
			"getcwd: cannot access parent directories", strerror(errno), 1);
		cwd[0] = '\0';
		ft_strappend2(cwd, data->pwd, PATH_MAX - 1);
		ft_strappend2(cwd, "/", PATH_MAX - 1);
		return (sh_update_env_var(data, ft_strappend2(cwd, dir, PATH_MAX - 1)));
	}
	if (ishyphen)
		ft_putendl_fd(cwd, 1);
	return (sh_update_env_var(data, cwd));
}

static int	sh_change_directory(char *dir, t_data *data)
{
	int		ishyphen;

	ishyphen = 0;
	if (!ft_strcmp(dir, "-"))
	{
		ishyphen = 1;
		if (!data->oldpwd_env)
		{
			return (sh_perror_return("\x1B[33mMinishell: \x1B[0m",
				"cd", "OLDPWD not set", 1));
		}
		dir = data->oldpwd_env;
	}
	if (chdir(dir) == -1)
	{
		return (sh_perror_return("\x1B[33mMinishell: \x1B[0mcd: ",
			dir, strerror(errno), 1));
	}
	return (sh_update(data, dir, ishyphen));
}

/*
**}
**	dir = ft_getcwd();
**	if (dir == NULL)
**	{
**		return (sh_perror_return("\x1B[31mMinishell: \x1B[0m",
**			"cd", strerror(errno), -1));
**	}
**	if (ishyphen)
**		ft_putendl_fd(dir, 1);
**	ishyphen = sh_update_env_var(data, dir);
**	free(dir);
**	return (ishyphen);
**}
*/

int			sh_cd(t_data *data, int n)
{
	int		i;

	if (!data->cmd[n]->argv[1])
	{
		i = 0;
		while (data->envp[i] && ft_strncmp(data->envp[i], "HOME=", 5))
			i++;
		if (!data->envp[i])
		{
			return (sh_perror_return("\x1B[33mMinishell: \x1B[0m",
				"cd", "HOME not set", 1));
		}
		if (!data->envp[i][5])
			return (0);
		return (sh_change_directory(data->envp[i] + 5, data));
	}
	else if (data->cmd[n]->argv[2])
	{
		return (sh_perror_return("\x1B[33mMinishell: \x1B[0m", "cd",
			"too many arguments", 1));
	}
	return (sh_change_directory(data->cmd[n]->argv[1], data));
}
