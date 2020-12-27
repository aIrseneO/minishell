/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 20:51:22 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/26 22:33:12 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		ft_execvp(char *line)
{
	char *ag[] = {"/bin/sh", "-c", line, NULL};
	if (!ft_strcmp(line, "exit"))
		exit(1);
	execvp(ag[0], ag);
	write(1, "Oops, something went wrong!\n", 28);
}

void		ft_execute_recursive_pipe(t_cmd *cmds, int fd, int i)
{
	int				pipefd[2];
	pid_t			father;

	if (!cmds->line2[i + 1])
	{
		if ((fd != STDIN_FILENO) && ((dup2(fd, STDIN_FILENO) == -1
					|| close(fd) == -1)))
			exit(-1);
		ft_execvp(cmds->line2[i]);
	}
	if ((pipe(pipefd) == -1 || (father = fork()) == -1))
		exit(-1);
	if (father == 0)
	{
		if (close(pipefd[0]) == -1 || dup2(fd, STDIN_FILENO) == -1
				|| dup2(pipefd[1], STDOUT_FILENO) == -1 ||
				close(pipefd[1]) == -1 || close(fd) == -1)
			exit(-1);
		ft_execvp(cmds->line2[i]);
	}
	if (wait(NULL) == -1 || close(pipefd[1]) == -1)
		exit(-1);
	ft_execute_recursive_pipe(cmds, pipefd[0], i + 1);
}
