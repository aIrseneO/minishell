/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 20:51:22 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/16 23:54:45 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			ft_execute_pipe(char *piped_cmds, char **envp)
{
	printf("\n\n|%s|\n\n", piped_cmds);
	execve("/bin/echo", envp, NULL);
	printf("see you\n");
	(void)piped_cmds; (void)envp;
	return (1);
}

static int			ft_execute_recursive_pipe(char **piped_cmds,
						char **envp, int fd)
{
	int				pipefd[2];
	int				wstatus;
	pid_t			child;

	if (!(*(piped_cmds + 1)))
	{
		if ((fd != STDIN_FILENO) && (dup2(fd, STDIN_FILENO) == -1
					|| close(fd) == -1))
			return (ft_perror("Error: dup2/close", -1, NULL));
		if (ft_execute_pipe(*piped_cmds, envp) == -1)
			return (ft_perror("Error: ft_execute_pipe", -1, NULL));
		return (1);
	}
	if ((pipe(pipefd) == -1 || (child = fork()) == -1))
		return (ft_perror("Error: pipe/fork", -1, NULL));
	if (child == 0)
	{
		if (close(pipefd[0]) == -1 || dup2(fd, STDIN_FILENO) == -1 ||
				dup2(pipefd[1], STDOUT_FILENO) == -1 || close(fd) == -1 ||
				close(pipefd[1]) == -1)
			return (ft_perror(strerror(errno), -1, NULL));
		if (ft_execute_pipe(*piped_cmds, envp) == -1)
			return (ft_perror("Error: ft_execute_pipe", -1, NULL));
	}
	if (child > 0)
	{
		if (waitpid(-1, &wstatus, 0) == -1 || close(pipefd[1]) == -1
				|| close(fd) == -1)
			return (ft_perror(strerror(errno), -1, NULL));
	}
	return (ft_execute_recursive_pipe(piped_cmds + 1, envp, pipefd[0]));
}

static int			ft_execute_cmd(char *cmd, char **envp)
{
	int				n;
	char			**piped_cmds;

	if ((piped_cmds = ft_split(cmd, '|')) == NULL)
		return (-1);
	n = ft_execute_recursive_pipe(piped_cmds, envp, STDIN_FILENO);
	if (n == -1)
		return (ft_perror("Error: ft_execute_recursive_pipe", -1, piped_cmds));
	return (n);
}

int					ft_execute_cmds(char *line, char **envp)
{
	int				i;
	int				n;
	char			**cmds;

	if ((cmds = ft_split(line, ';')) == NULL)
		return (ft_perror("Error: ft_split", -1, NULL));
	i = 0;
	while (cmds[i])
	{
		n = ft_execute_cmd(cmds[i++], envp);
		if (n == -1)
			return (ft_perror("Error: ft_execute_cmd", -1, cmds));
		else if (n == 0)
			return (ft_perror(NULL, 0, cmds));
	}
	return (1);
}
