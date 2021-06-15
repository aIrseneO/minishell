/******************************************************************************/
/*     Copyright (c) 2020 Arsene Temfack                                      */
/*                                                                            */
/*     SPDX-License-Identifier: MIT                                           */
/******************************************************************************/

#include "minishell.h"

void	sh_free_data_exit1(t_data *data, char *s1, char *s2, int exno)
{
	write(STDERR_FILENO, RED, 5);
	write(STDERR_FILENO, "Minishell: ", 11);
	write(STDERR_FILENO, RESET, 4);
	if (s1)
	{
		write(STDERR_FILENO, s1, ft_strlen(s1));
		write(STDERR_FILENO, ": ", 2);
	}
	if (s2)
		write(STDERR_FILENO, s2, ft_strlen(s2));
	write(STDERR_FILENO, "\n", 1);
	if (data)
		sh_free_t_data(data);
	exit(exno);
}

void	sh_free_data_exit2(t_data *data, char *s1, char *s2, int exno)
{
	write(STDERR_FILENO, YEL, 5);
	write(STDERR_FILENO, "Minishell: ", 11);
	write(STDERR_FILENO, RESET, 4);
	if (s1)
	{
		write(STDERR_FILENO, s1, ft_strlen(s1));
		write(STDERR_FILENO, ": ", 2);
	}
	if (s2)
		write(STDERR_FILENO, s2, ft_strlen(s2));
	write(STDERR_FILENO, "\n", 1);
	if (data)
		sh_free_t_data(data);
	exit(exno);
}

int		sh_perror_return(char *s1, char *s2, char *s3, int n)
{
	write(STDERR_FILENO, s1, ft_strlen(s1));
	if (s2)
	{
		write(STDERR_FILENO, s2, ft_strlen(s2));
		write(STDERR_FILENO, ": ", 2);
	}
	if (s3)
		write(STDERR_FILENO, s3, ft_strlen(s3));
	write(STDERR_FILENO, "\n", 1);
	return (n);
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
	free(data->pwd_env);
	free(data->oldpwd_env);
	free(data->line);
	ft_astrfree(&data->line1, free);
	ft_astrfree(&data->line2, free);
	free(data->children_pid);
	ft_lstclear(data->envpl, free);
	free(data->envpl);
	free(data->envp);
	ft_astrfree(&data->path, free);
	sh_cmdfree(&data->cmd);
}
