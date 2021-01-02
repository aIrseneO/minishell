/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:02:28 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/02 00:46:56 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		ft_link1(char *file)
{
	int			fd;

	if ((fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0666)) == -1)
		ft_perror_exit("\x1B[33mMinishell: \x1B[0m", file, strerror(errno), 1);
	if (dup2(fd, STDOUT_FILENO) == -1 || close(fd) == -1)
		ft_perror_exit("\x1B[31mMinishell: \x1B[0m", NULL, strerror(errno), 1);
}

static void		ft_link2(char *file)
{
	int			fd;
	DIR			*dir;
	struct stat	sb;

	if (stat(file, &sb) == -1)
		ft_perror_exit("\x1B[33mMinishell: \x1B[0m", file, strerror(errno), 1);
	if (!S_ISREG(sb.st_mode) && !S_ISDIR(sb.st_mode) &&
			!S_ISLNK(sb.st_mode) && !S_ISFIFO(sb.st_mode))
		ft_perror_exit("\x1B[33mMinishell: \x1B[0m", file,
			": \x1B[35mNot handled\x1B[0m\n", 1);
	if (S_ISDIR(sb.st_mode))
	{
		if ((dir = opendir(file)) == NULL)
			ft_perror_exit("\x1B[33mMinishell: \x1B[0m", file,
				strerror(errno), 1);
		if ((fd = dirfd(dir)) == -1)
			ft_perror_exit("\x1B[31mMinishell: \x1B[0m", NULL,
				strerror(errno), 1);
	}
	else if ((fd = open(file, O_RDONLY)) == -1)
		ft_perror_exit("\x1B[33mMinishell: \x1B[0m", file, strerror(errno), 1);
	if (dup2(fd, STDOUT_FILENO) == -1 || close(fd) == -1)
		ft_perror_exit("\x1B[31mMinishell: \x1B[0m", NULL, strerror(errno), 1);
}

static void		ft_link3(char *file)
{
	int			fd;

	if ((fd = open(file, O_WRONLY|O_CREAT|O_APPEND)) == -1)
		ft_perror_exit("\x1B[33mMinishell: \x1B[0m", file, strerror(errno), 1);
	if (dup2(fd, STDOUT_FILENO) == -1 || close(fd) == -1)
		ft_perror_exit("\x1B[31mMinishell: \x1B[0m", NULL, strerror(errno), 1);
}

void			ft_execute_recursive_redirection(t_data data, int i)
{
	if (!ft_strcmp(data.cmd[i]->redirections->content, ">"))
		ft_link1(data.cmd[i]->files->content);
	else if (!ft_strcmp(data.cmd[i]->redirections->content, "<"))
		ft_link2(data.cmd[i]->files->content);
	else if (!ft_strcmp(data.cmd[i]->redirections->content, ">>"))
		ft_link3(data.cmd[i]->files->content);
	else if (!ft_strcmp(data.cmd[i]->redirections->content, "<<"))
		ft_perror_exit("\x1B[33mMinishell: \x1B[0m", "<< ",
				": \x1B[35mNot handled\x1B[0m\n", 1);
	else if (!ft_strcmp(data.cmd[i]->redirections->content, "<>"))
	{
		if (close(open(data.cmd[i]->files->content, O_RDONLY|O_CREAT)) == -1)
			ft_perror_exit("\x1B[33mMinishell: \x1B[0m",
				data.cmd[i]->files->content, strerror(errno), 1);
	}
	else
		ft_perror_exit("Oops, something went wrong!\n", NULL, NULL, -1);
	if (!data.cmd[i]->redirections->next)
		ft_execve(data, i);
	data.cmd[i]->redirections =	data.cmd[i]->redirections->next;
	data.cmd[i]->files = data.cmd[i]->files->next;
	ft_execute_recursive_redirection(data, i);
}
