/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_execute_utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 23:49:51 by atemfack          #+#    #+#             */
/*   Updated: 2021/03/12 00:14:06 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char			*sh_get_absolute_path(char *app, t_data *data, int j)
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
					return (ft_strjoin2(data->path[j], "/", app));
				}
			}
			closedir(directory);
		}
		j++;
	}
	sh_free_data_exit2(data, app, "command not found", 127);
	return (NULL);
}

static void			sh_update_path(t_data *data, int i, char *path)
{
	if (path == NULL)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
	free(data->cmd[i]->argv[0]);
	data->cmd[i]->argv[0] = path;
}

void				sh_check_and_update_path(t_data *data, int i, char *app)
{
	if (*app == '.' || *app == '/' || !data->path) 
		sh_update_path(data, i, ft_strdup(app));
	else
		sh_update_path(data, i , sh_get_absolute_path(app, data, 0));
}

/*
** There is more to handle here in order to mimic bash
*/

void				sh_handle_execve_error(t_data *data, int i, char *app)
{
	struct stat		sb;

	if (stat(app, &sb) == -1)
		sh_free_data_exit2(data, app, "No such file or directory", 127);
	if (S_ISDIR(sb.st_mode))
		sh_free_data_exit2(data, app, "Is a directory", 126);
	if (errno == EACCES)
		sh_free_data_exit2(data, app, "Permission denied", 126);
	sh_free_data_exit2(data, data->cmd[i]->app, strerror(errno), errno);
}
