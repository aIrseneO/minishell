/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/27 23:07:07 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/28 01:10:09 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int					ft_isfathercmd(char *cmd)
{
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (2);
	if (!ft_strcmp(cmd, "export"))
		return (3);
	if (!ft_strcmp(cmd, "unset"))
		return (4);
	return (0);
}

int					ft_isredirection(char c)
{
	return (c == '<' || c == '>');
}

t_list		*ft_new_list(char *begin, char *end)
{
	int				n;
	t_list			*new;
	char			*tmp;

	n = end - begin;
	if ((tmp = (char *)malloc(sizeof(*tmp) * (n + 1))) == NULL)
		return (NULL);
	ft_strlcpy(tmp, begin, n + 1);
	tmp[n] = '\0';
	if ((new = ft_lstnew(tmp)) == NULL)
	{
		free(tmp);
		return (NULL);
	}
	return (new);
}
