/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 20:51:22 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/13 20:11:31 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*			Made for testing
	static void		sh_execvp(char *line)
	{
		char *ag[] = {"/bin/sh", "-c", line, NULL};
		if (!ft_strcmp(line, "exit"))
			exit(1);
		execvp(ag[0], ag);
		write(1, "Oops, something went wrong!\n", 28);
	}
*/

static int	sh_load_path(char *path, char *path1, char *app)
{
	while (*path1)
		*path++ = *path1++;
	*path++ = '/';
	while (*app)
		*path++ = *app++;
	*path = '\0';
	return (1);
}

static int	sh_get_path(char *path, t_data data, int i)
{
	int				j;
	DIR				*directory;
	struct dirent	*dir;

	j = 0;
	while (data.path[j])
	{
		directory = opendir(data.path[j]);
		if (directory)
		{
			while (1)
			{
				dir = readdir(directory);
				if (dir == NULL)
					break ;
				if (!ft_strcmp(dir->d_name, data.cmd[i]->app))
					return (sh_load_path(path, data.path[j], dir->d_name));
			}
			closedir(directory);
		}
		j++;
	}
	return (0);
}

void	sh_execve(t_data data, int i)
{
	char			path[PATH_MAX];

	if (!data.path || !(sh_get_path(path, data, i)))
		sh_perror_exit(YEL, data.cmd[i]->app, "command not found", 127);
	data.cmd[i]->argv[0] = path;
	if (execve(path, data.cmd[i]->argv, data.envp) == -1)
		sh_perror_exit(YEL, data.cmd[i]->app, strerror(errno), 127);
	write(1, "Oops, something went wrong!\n", 28);
}

static void	sh_execute(t_data data, int i)
{
	if (data.cmd[i]->redirections)
		sh_execute_recursive_redirection(data, i);
	sh_execve(data, i);
}

void	sh_execute_recursive_pipe(t_data data, int fd, int i)
{
	int			pipefd[2];
	pid_t		father;

	if (!data.line2[i + 1])
	{
		if ((fd != STDIN_FILENO)
				&& ((dup2(fd, STDIN_FILENO) == -1 || close(fd) == -1)))
			sh_perror_exit(RED, NULL, strerror(errno), -1);
		sh_execute(data, i);
	}
	if (pipe(pipefd) == -1)
		sh_perror_exit(RED, NULL, strerror(errno), -1);
	father = fork();
	if (father == -1)
		sh_perror_exit(RED, NULL, strerror(errno), -1);
	if (father == 0)
	{
		if (close(pipefd[0]) == -1 || dup2(fd, STDIN_FILENO) == -1
			|| dup2(pipefd[1], STDOUT_FILENO) == -1
			|| close(pipefd[1]) == -1 || close(fd) == -1)
			sh_perror_exit(RED, NULL, strerror(errno), -1);
		sh_execute(data, i);
	}
	if (wait(NULL) == -1 || close(pipefd[1]) == -1)
		sh_perror_exit(RED, NULL, strerror(errno), -1);
	sh_execute_recursive_pipe(data, pipefd[0], i + 1);
}
