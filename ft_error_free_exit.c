/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_free_exit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 18:17:44 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/03 23:39:27 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_perror_exit(char *s1, char *s2, char *s3, int n)
{
	write(1, s1, ft_strlen(s1));
	if (s2)
	{
		write(1, s2, ft_strlen(s2));
		write(1, " : ", 3);
	}
	if (s3)
		write(1, s3, ft_strlen(s3));
	write(1, "\n", 1);
	exit(n);
}

int			ft_perror_return(char *s1, char *s2, char *s3, int n)
{
	write(1, s1, ft_strlen(s1));
	if (s2)
	{
		write(1, s2, ft_strlen(s2));
		write(1, " : ", 3);
	}
	if (s3)
		write(1, s3, ft_strlen(s3));
	write(1, "\n", 1);
	return(n);
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
