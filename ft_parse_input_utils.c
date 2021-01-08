/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/27 23:07:07 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/07 09:08:50 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list		*ft_new_list(char *begin, char *end)
{
	int				n;
	t_list			*new;
	char			*tmp;

	n = end - begin;
	if ((tmp = (char *)malloc(sizeof(*tmp) * (n + 2))) == NULL)
		return (NULL);
	ft_strlcpy(tmp, begin, n + 2);
	tmp[n + 1] = '\0';
	if ((new = ft_lstnew(tmp)) == NULL)
	{
		free(tmp);
		return (NULL);
	}
	return (new);
}

int					ft_load_argv(char ***argv, t_list *args)
{
	int				i;

	if (!((*argv) = ft_astrinit(ft_lstsize(args) + 2)))
		return (-1);
	i = 0;
	if (!((*argv)[i++] = ft_strdup("42")))
		return (-1);
	while (args)
	{
		if (!((*argv)[i++] = ft_strdup(args->content)))
			return (-1);
		args = args->next;
	}
	return (1);
}
