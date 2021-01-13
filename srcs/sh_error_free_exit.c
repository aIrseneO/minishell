/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_error_free_exit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 18:17:44 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/12 11:40:39 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sh_perror_exit(char *s1, char *s2, char *s3, int n)
{
	if (s1)
	{
		write(1, s1, ft_strlen(s1));
		write(1, "Minishell: ", 11);
		write(1, RESET, 5);
	}
	if (s2)
	{
		write(1, s2, ft_strlen(s2));
		write(1, ": ", 2);
	}
	if (s3)
		write(1, s3, ft_strlen(s3));
	write(1, "\n", 1);
	exit(n);
}

int	sh_perror_return(char *s1, char *s2, char *s3, int n)
{
	write(1, s1, ft_strlen(s1));
	if (s2)
	{
		write(1, s2, ft_strlen(s2));
		write(1, ": ", 3);
	}
	if (s3)
		write(1, s3, ft_strlen(s3));
	write(1, "\n", 1);
	return (n);
}

int	sh_perror_free_t_data(char *_errmsg, t_data *data)
{
	if (_errmsg)
	{
		write(1, RED, 5);
		write(1, _errmsg, ft_strlen(_errmsg));
		write(1, RESET, 4);
	}
	if (data)
		sh_free_t_data(data);
	return (errno);
}

void	sh_cmdfree(t_cmd ***cmd)
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
		ft_astrfree(&(*cmd)[i]->argv, free);
		free((*cmd)[i++]);
	}
	free(*cmd);
	*cmd = NULL;
}

void	sh_free_t_data(t_data *data)
{
	free(data->pwd);
	free(data->line);
	ft_astrfree(&data->line1, free);
	ft_astrfree(&data->line2, free);
	ft_astrfree(&data->envp, free);
	ft_astrfree(&data->path, free);
	sh_cmdfree(&data->cmd);
}
