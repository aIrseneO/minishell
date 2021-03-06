/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_unset1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 16:04:15 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/11 05:31:34 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sh_update_env_var(t_data *data, char *argv)
{
	if (!ft_strcmp(argv, "PATH"))
		ft_astrfree(&data->path, free);
	if (!ft_strcmp(argv, "PWD"))
	{
		data->ispwd_env = 0;
		ft_strdel(&data->pwd_env);
	}
	if (!ft_strcmp(argv, "OLDPWD"))
	{
		data->isoldpwd_env = 0;
		ft_strdel(&data->oldpwd_env);
	}
}

static void	sh_removep(t_data *data, char *argv, int *isremoved)
{
	t_list	*tmp1;
	t_list	*tmp2;

	sh_update_env_var(data, argv);
	tmp1 = *data->envpl;
	tmp2 = *data->envpl;
	while (tmp1 && ft_strncmp(tmp1->content, argv, ft_strlen(argv)))
	{
		tmp2 = tmp1;
		tmp1 = tmp1->next;
	}
	if (!tmp1)
		return ;
	*isremoved = 1;
	if (tmp1 == *data->envpl)
	{
		tmp2 = tmp1->next;
		ft_lstdelone(tmp1, free);
		*data->envpl = tmp2;
		return ;
	}
	tmp1 = tmp2->next->next;
	ft_lstdelone(tmp2->next, free);
	tmp2->next = tmp1;
}

static void	sh_puterr_set_n(char *argv, int *n)
{
	*n = 1;
	ft_putstr_fd("\x1B[33mMinishell: \x1B[0munset: `", STDERR_FILENO);
	ft_putstr_fd(argv, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

int			sh_unset(t_data *data, int i)
{
	int		n;
	int		isremoved;
	char	**argv;

	n = 0;
	isremoved = 0;
	argv = data->cmd[i]->argv + 1;
	while (*argv)
	{
		if (!ft_isname(*argv))
			sh_puterr_set_n(*argv, &n);
		else
			sh_removep(data, *argv, &isremoved);
		argv++;
	}
	if (!isremoved)
		return (n);
	argv = ft_lsttoastr(*data->envpl);
	if (argv == NULL && *data->envpl)
		sh_free_data_exit1(data, NULL, strerror(errno), -1);
	free(data->envp);
	data->envp = argv;
	return (n);
}
