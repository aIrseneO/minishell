/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 20:51:22 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/12 16:12:22 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Made for testing
**static void		sh_execvp(char *line)
**{
**	char *ag[] = {"/bin/sh", "-c", line, NULL};
**	if (!ft_strcmp(line, "exit"))
**		exit(1);
**	execvp(ag[0], ag);
**	write(STDERR_FILENO, "Oops, something went wrong!\n", 28);
**}
*/

int				sh_execve(t_data *data, int i)
{
	struct stat	sb;
	char		*tmp;

	if (!data->cmd[i]->app)
		exit(0);
	if (!ft_strcmp(data->cmd[i]->app, "."))
	{
		write(STDERR_FILENO,
			"\x1B[33mMinishell\x1B[0m: .: filename argument required\n", 50);
		write(STDERR_FILENO,
			".: \x1B[33musage\x1B[0m: . filename [arguments]\n", 42);
		exit(2);
	}
	sh_execute_if_non_binary(data, i);
	tmp = ft_strdup(data->cmd[i]->app);
	if (tmp == NULL)
		sh_free_data_exit1(data, data->cmd[i]->app, strerror(errno), 127);
	free(data->cmd[i]->argv[0]);
	data->cmd[i]->argv[0] = tmp;
	if (stat(data->cmd[i]->app, &sb) == -1)
		sh_free_data_exit2(data, data->cmd[i]->app, "command not found", 127);
	if (execve(data->cmd[i]->app, data->cmd[i]->argv, data->envp) == -1)
		sh_free_data_exit1(data, data->cmd[i]->app, strerror(errno), -1);
	write(STDERR_FILENO, "Oops, something went wrong!\n", 28);
	return (-1);
}

static void		sh_execute_last_pipe(t_data *data, int fd, int i)
{
	if ((fd != STDIN_FILENO)
			&& ((dup2(fd, STDIN_FILENO) == -1 || close(fd) == -1)))
		sh_free_data_exit1(data, NULL, strerror(errno), -1);
	data->children_pid[i] = fork();
	if (data->children_pid[i] == -1)
		sh_free_data_exit1(data, NULL, strerror(errno), -1);
	if (data->children_pid[i] == 0)
		sh_recursive_redirection(data, i, sh_execve);
	if (data->ispiped)
		sh_restore_std_fileno(data->std_fileno);
}

void			sh_execute_pipes(t_data *data, int fd, int i)
{
	int			pipefd[2];

	while (data->line2[i + 1])
	{
		if (pipe(pipefd) == -1)
			sh_free_data_exit1(data, NULL, strerror(errno), -1);
		data->children_pid[i] = fork();
		if (data->children_pid[i] == -1)
			sh_free_data_exit1(data, NULL, strerror(errno), -1);
		if (data->children_pid[i] == 0)
		{
			if ((fd != STDIN_FILENO && dup2(fd, STDIN_FILENO) == -1)
					|| dup2(pipefd[1], STDOUT_FILENO) == -1
					|| close(pipefd[0]) == -1 || close(pipefd[1]) == -1
					|| (fd != STDIN_FILENO && close(fd) == -1))
				sh_free_data_exit1(data, NULL, strerror(errno), -1);
			sh_recursive_redirection(data, i, sh_execve);
		}
		if (close(pipefd[1]) == -1 || (fd != STDIN_FILENO && close(fd) == -1))
			sh_free_data_exit1(data, NULL, strerror(errno), -1);
		fd = pipefd[0];
		i++;
	}
	sh_execute_last_pipe(data, fd, i);
}

/*
**void			sh_recursive_pipe(t_data *data, int fd, int i)
**{
**	int			pipefd[2];
**	pid_t		father;
**
**	if (!data->line2[i + 1])
**	{
**		if ((fd != STDIN_FILENO)
**			&& ((dup2(fd, STDIN_FILENO) == -1 || close(fd) == -1)))
**			sh_free_data_exit1(data, NULL, strerror(errno), -1);
**		sh_recursive_redirection(data, i, sh_execve);
**	}
**	if (pipe(pipefd) == -1)
**		sh_free_data_exit1(data, NULL, strerror(errno), -1);
**	father = fork();
**	if (father == -1)
**		sh_free_data_exit1(data, NULL, strerror(errno), -1);
**	if (father == 0)
**	{
**		if ((fd != STDIN_FILENO && dup2(fd, STDIN_FILENO) == -1)
**				|| dup2(pipefd[1], STDOUT_FILENO) == -1
**				|| close(pipefd[0]) == -1 || close(pipefd[1]) == -1
**				|| (fd != STDIN_FILENO && close(fd) == -1))
**			sh_free_data_exit1(data, NULL, strerror(errno), -1);
**		sh_recursive_redirection(data, i, sh_execve);
**	}
**	if (signal(SIGINT, sig_handler2) == SIG_ERR ||
**			signal(SIGQUIT, sig_handler1) == SIG_ERR)
**		sh_free_data_exit1(data, NULL, strerror(errno), -1);
**	if (waitpid(father, NULL, 0) == -1)
**		sh_free_data_exit1(data, NULL, strerror(errno), -1);
**	if (close(pipefd[1]) == -1)
**		sh_free_data_exit1(data, NULL, strerror(errno), -1);
**	sh_recursive_pipe(data, pipefd[0], i + 1);
**}
*/
