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

//			Made for testing
/*static void		sh_execvp(char *line)
{
	char *ag[] = {"/bin/sh", "-c", line, NULL};
	if (!ft_strcmp(line, "exit"))
		exit(1);
	execvp(ag[0], ag);
	write(STDERR_FILENO, "Oops, something went wrong!\n", 28);
}*/

int	sh_execve(t_data *data, int i)
{
	struct stat		sb;

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
	sh_execute_if_father_app(data, i);
	free(data->cmd[i]->argv[0]);
	data->cmd[i]->argv[0] = data->cmd[i]->app;
	if (stat(data->cmd[i]->app, &sb) == -1)
		sh_perror_exit(YEL, data->cmd[i]->app, "command not found", 127);
	if (execve(data->cmd[i]->app, data->cmd[i]->argv, data->envp) == -1)
		sh_perror_exit(YEL, data->cmd[i]->app, strerror(errno), 127);
	write(STDERR_FILENO, "Oops, something went wrong!\n", 28);
	return (-42);
}

void	sh_execute_recursive_pipe(t_data *data, int fd, int i, pid_t father)
{
	int			pipefd[2];

	if (!data->line2[i + 1])
	{
		if ((fd != STDIN_FILENO)
			&& ((dup2(fd, STDIN_FILENO) == -1 || close(fd) == -1)))
			sh_perror_exit(RED, NULL, strerror(errno), -1);
		sh_recursive_redirection(data, i, sh_execve);
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
		sh_recursive_redirection(data, i, sh_execve);
	}
	if (wait(NULL) == -1 || close(pipefd[1]) == -1)
		sh_perror_exit(RED, NULL, strerror(errno), -1);
	sh_execute_recursive_pipe(data, pipefd[0], i + 1, 0);
}
