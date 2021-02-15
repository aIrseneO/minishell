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

static void	sh_update_env_var(char *argv, t_data *data)
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

static void	sh_removep(t_list **list, char *argv, int *isremoved)
{
	t_list	*tmp1;
	t_list	*tmp2;

	tmp1 = *list;
	tmp2 = *list;
	while (tmp1 && ft_strncmp(tmp1->content, argv, ft_strlen(argv)))
	{
		tmp2 = tmp1;
		tmp1 = tmp1->next;
	}
	if (!tmp1)
		return ;
	*isremoved = 1;
	if (tmp1 == *list)
	{
		tmp2 = tmp1->next;
		ft_lstdelone(tmp1, free);
		*list = tmp2;
		return ;
	}
	tmp1 = tmp2->next->next;
	ft_lstdelone(tmp2->next, free);
	tmp2->next = tmp1;
}

int	sh_unset(t_data *data, int i)
{
	int		isremoved;
	char	**argv;

	isremoved = 0;
	argv = data->cmd[i]->argv + 1;
	while (*argv)
	{
		sh_update_env_var(*argv, data);
		sh_removep(data->envpl, *argv++, &isremoved);
	}
	if (!isremoved)
		return (0);
	argv = ft_lsttoastr(*data->envpl);
	if (argv == NULL && *data->envpl)
		return (sh_perror_return("\x1B[31mMinishell: \x1B[0m", "unset",
				strerror(errno), 1));
	free(data->envp);
	data->envp = argv;
	return (0);
}
