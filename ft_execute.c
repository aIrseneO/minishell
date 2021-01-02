/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 20:51:22 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/01 22:40:06 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	static void		ft_execvp(char *line)
**	{
**		char *ag[] = {"/bin/sh", "-c", line, NULL};
**		if (!ft_strcmp(line, "exit"))
**			exit(1);
**		execvp(ag[0], ag);
**		write(1, "Oops, something went wrong!\n", 28);
**	}
*/

void		ft_execve(t_data data, int i)
{
	//char		path[PATH_MAX];

	char *path;
	path = ft_strjoin(ft_strjoin(data.path[0], "/"), data.cmd[i]->app);
	//ft_printf("%s\n", path);
	//ft_astrprint(data.cmd[i]->argv);
	//ft_astrprint(data.envp);

	data.cmd[i]->argv[0] = path;
	execve(path, data.cmd[i]->argv, data.envp);
	write(1, "Oops, something went wrong!\n", 28);
}

static void		ft_execute(t_data data, int i)
{
	if (data.cmd[i]->redirections)
		ft_execute_recursive_redirection(data, i);
	ft_execve(data, i);
}

void			ft_execute_recursive_pipe(t_data data, int fd, int i)
{
	int			pipefd[2];
	pid_t		father;

	if (!data.line2[i + 1])
	{
		if ((fd != STDIN_FILENO) && ((dup2(fd, STDIN_FILENO) == -1
					|| close(fd) == -1)))
			exit(-1);
		ft_execute(data, i);
	}
	if ((pipe(pipefd) == -1 || (father = fork()) == -1))
		exit(-1);
	if (father == 0)
	{
		if (close(pipefd[0]) == -1 || dup2(fd, STDIN_FILENO) == -1
				|| dup2(pipefd[1], STDOUT_FILENO) == -1 ||
				close(pipefd[1]) == -1 || close(fd) == -1)
			exit(-1);
		ft_execute(data, i);
	}
	if (wait(NULL) == -1 || close(pipefd[1]) == -1)
		exit(-1);
	ft_execute_recursive_pipe(data, pipefd[0], i + 1);
}
