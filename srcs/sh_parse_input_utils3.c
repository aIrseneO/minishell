/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_input_utils3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 23:49:51 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/10 13:38:34 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*sh_get_absolute_path(char *app, t_data *data, int j)
{
	DIR				*directory;
	struct dirent	*dir;

	while (data->path[j])
	{
		directory = opendir(data->path[j]);
		if (directory)
		{
			while (1)
			{
				dir = readdir(directory);
				if (dir == NULL)
					break ;
				if (!ft_strcmp(dir->d_name, app))
				{
					closedir(directory);
					return (ft_strjoin2(data->path[j], "/", dir->d_name));
				}
			}
			closedir(directory);
		}
		j++;
	}
	return (app);
}

static int	sh_is_builtin(char *app)
{
	if (!ft_strcmp(app, "cd") || !ft_strcmp(app, "export")
		|| !ft_strcmp(app, "unset") || !ft_strcmp(app, "pwd")
		|| !ft_strcmp(app, "exit"))
		return (1);
	return (0);
}

int	sh_make_absolute_path(char **app, t_data *data)
{
	char	*path;

	if (!data->path)
		return (0);
	if (**app == '.' || **app == '/')
		return (0);
	if (sh_is_builtin(*app))
		return (0);
	path = sh_get_absolute_path(*app, data, 0);
	if (path == NULL)
		return (-1);
	if (path == *app)
		return (0);
	free(*app);
	*app = path;
	return (0);
}
