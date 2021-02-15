/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_execute_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:02:28 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/12 15:57:22 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sh_link1(char *file)
{
	int			fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		sh_perror_exit(YEL, file, strerror(errno), 1);
	if (dup2(fd, STDOUT_FILENO) == -1 || close(fd) == -1)
		sh_perror_exit(RED, NULL, strerror(errno), 1);
}

static void	sh_link2(char *file)
{
	int			fd;
	struct stat	sb;

	if (stat(file, &sb) == -1)
		sh_perror_exit(YEL, file, strerror(errno), 1);
	if (!S_ISREG(sb.st_mode) && !S_ISDIR(sb.st_mode)
		&& !S_ISLNK(sb.st_mode) && !S_ISFIFO(sb.st_mode))
		sh_perror_exit(YEL, file, "\x1B[35mNot handled yet\x1B[0m\n", 1);
	if (S_ISDIR(sb.st_mode))
		fd = STDIN_FILENO;
	else
	{
		fd = open(file, O_RDONLY);
		if (fd == -1)
			sh_perror_exit(YEL, file, strerror(errno), 1);
	}
	if (dup2(fd, STDIN_FILENO) == -1 || close(fd) == -1)
		sh_perror_exit(RED, NULL, strerror(errno), 1);
}

static void	sh_link3(char *file)
{
	int			fd;

	fd = open(file, O_APPEND | O_CREAT | O_WRONLY, 0666 );
	if (fd == -1)
		sh_perror_exit(YEL, file, strerror(errno), 1);
	if (dup2(fd, STDOUT_FILENO) == -1 || close(fd) == -1)
		sh_perror_exit(RED, NULL, strerror(errno), 1);
}

void	sh_recursive_redirection(t_data *data, int i, int (*exec)(t_data*, int))
{
	if (!data->cmd[i]->redirections && !data->cmd[i]->files)
	{
		data->status = exec(data, i);
		return ;
	}
	if (!ft_strcmp(data->cmd[i]->redirections->content, ">"))
		sh_link1(data->cmd[i]->files->content);
	else if (!ft_strcmp(data->cmd[i]->redirections->content, "<"))
		sh_link2(data->cmd[i]->files->content);
	else if (!ft_strcmp(data->cmd[i]->redirections->content, ">>"))
		sh_link3(data->cmd[i]->files->content);
	else if (!ft_strcmp(data->cmd[i]->redirections->content, "<<"))
		sh_perror_exit(YEL, "<< ", "\x1B[35mNot handled yet\x1B[0m\n", 1);
	else if (!ft_strcmp(data->cmd[i]->redirections->content, "<>"))
	{
		if (close(open(data->cmd[i]->files->content, O_RDONLY | O_CREAT)) == -1)
			sh_perror_exit(YEL, data->cmd[i]->files->content,
				strerror(errno), 1);
	}
	else
		sh_perror_exit(NULL, NULL, "Oops, something went wrong!\n", -1);
	data->cmd[i]->redirections = data->cmd[i]->redirections->next;
	data->cmd[i]->files = data->cmd[i]->files->next;
	sh_recursive_redirection(data, i, exec);
}
