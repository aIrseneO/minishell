/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 18:17:44 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/02 00:47:46 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_perror_exit(char *shell, char *file, char *strerr, int exit_nbr)
{
	write(1, shell, ft_strlen(shell));
	if (file)
	{
		write(1, file, ft_strlen(file));
		write(1, " : ", 3);
	}
	if (strerr)
		write(1, strerr, ft_strlen(strerr));
	write(1, "\n", 1);
	exit(exit_nbr);
}

int			ft_perror(char *_errmsg, t_data *data)
{
	if (_errmsg)
	{
		write(1, RED, 5);
		write(1, _errmsg, ft_strlen(_errmsg));
		write(1, RESET, 4);
	}
	if (data)
		ft_free_t_data(data);
	return (errno);
}

void		ft_cmdfree(t_cmd ***cmd)
{
	int		i;

	if (!(*cmd))
		return ;
	i = 0;
	while ((*cmd)[i])
	{
		free((*cmd)[i]->app);
		ft_lstclear(&(*cmd)[i]->args, free);
		ft_lstclear(&(*cmd)[i]->redirections, free);
		ft_lstclear(&(*cmd)[i]->files, free);
		ft_astrfree(&(*cmd)[i]->argv);
		free((*cmd)[i++]);
	}
	free(*cmd);
	*cmd = NULL;
}

void		ft_free_t_data(t_data *data)
{
	free(data->pwd);
	free(data->line);
	ft_astrfree(&data->line1);
	ft_astrfree(&data->line2);
	ft_astrfree(&data->envp);
	ft_astrfree(&data->path);
	ft_cmdfree(&data->cmd);
}
