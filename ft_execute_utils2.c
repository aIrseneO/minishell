/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 22:05:48 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/08 03:05:59 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			ft_load_path(char *path, char *path1, char *app)
{
	while (*path1)
		*path++ = *path1++;
	*path++ = '/';
	while (*app)
		*path++ = *app++;
	*path = '\0';
	return (1);
}

static int			ft_get_path(char *path, t_data data, int i)
{
	int				j;
	DIR				*directory;
	struct dirent	*dir;

	j = 0;
	while (data.path[j])
	{
		if ((directory = opendir(data.path[j])))
		{
			while ((dir = readdir(directory)) != NULL)
				if (!ft_strcmp(dir->d_name, data.cmd[i]->app))
					return (ft_load_path(path, data.path[j], dir->d_name));
			closedir(directory);
		}
		j++;
	}
	return (0);
}

void				ft_execve(t_data data, int i)
{
	char			path[PATH_MAX];
	
	if (ft_replace_envp_in_cmd(&data.cmd[i]->app,
			&data.cmd[i]->argv, data.envp) == -1)
		ft_perror_exit("\x1B[31mMinishell: \x1B[0m", NULL, strerror(errno), -1);
	if (!data.path || !(ft_get_path(path, data, i)))
		ft_perror_exit("\x1B[33mMinishell: \x1B[0m", data.cmd[i]->app, 
				"command not found", 127);
	data.cmd[i]->argv[0] = path;
	if (execve(path, data.cmd[i]->argv, data.envp) == -1)
		ft_perror_exit("\x1B[33mMinishell: \x1B[0m", data.cmd[i]->app, 
				strerror(errno), 127);
	write(1, "Oops, something went wrong!\n", 28);
}
