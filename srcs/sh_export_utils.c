/*
* Copyright (c) 2020 Arsene Temfack
*
* SPDX-License-Identifier: MIT
*/

#include "minishell.h"

static int	sh_replace_next_envpl(t_list *list, char *str, t_list **lst)
{
	t_list	*tmp1;
	t_list	*tmp2;

	tmp2 = ft_lstnew2(str);
	if (!tmp2)
		return (-1);
	if (!list)
	{
		tmp1 = (*lst)->next;
		ft_lstdelone((*lst), free);
		tmp2->next = tmp1;
		(*lst) = tmp2;
		return (0);
	}
	tmp1 = list->next->next;
	ft_lstdelone(list->next, free);
	list->next = tmp2;
	tmp2->next = tmp1;
	return (0);
}

static int	sh_export_str_envpl(char *str, t_list **list, int n)
{
	t_list	*tmp1;
	t_list	*tmp2;

	tmp1 = *list;
	tmp2 = NULL;
	while (tmp1 && ft_strncmp(tmp1->content, str, n))
	{
		tmp2 = tmp1;
		tmp1 = tmp1->next;
	}
	if (tmp1)
		return (sh_replace_next_envpl(tmp2, str, list));
	tmp1 = ft_lstnew2(str);
	if (!tmp1)
		return (-1);
	if (!(*list))
		ft_lstadd_back(list, tmp1);
	else
		ft_lstadd_back(&tmp2, tmp1);
	return (0);
}

/*
** `n` is the number of char before `=`
*/

int			sh_export_str(t_data *data, char *str, int n)
{
	char	**argv;

	if (n == 0)
		return (0);
	if (sh_export_str_envpl(str, data->envpl, n + 1) == -1)
		return (-1);
	argv = ft_lsttoastr(*data->envpl);
	if (argv == NULL)
		return (-1);
	ft_astrfree(&data->envp, NULL);
	data->envp = argv;
	return (0);
}
