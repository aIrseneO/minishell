/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 18:11:48 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/01 19:55:16 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO Put in libft

char				*ft_getcwd(void)
{
	char			buf[PATH_MAX];

	if (getcwd(buf, PATH_MAX) == NULL)
		return (NULL);
	return (ft_strdup(buf));
}

void			ft_astrprint(char **s)
{
	ft_printf("\n-->");
	if (!s)
	{
		ft_printf("<--\n");
		return ;
	}
	while (*s)
		ft_printf("%s, ", *s++);
	ft_printf("<--\n");
}

char				**ft_astrinit(int size)
{
	char			**str;

	if ((str = (char **)malloc(sizeof(*str) * size)) == NULL)
		return (NULL);
	while (--size >= 0)
		str[size] = NULL;
	return (str);
}

void				ft_astrfree(char ***str)
{
	int				i;

	if (!str || !(*str))
		return ;
	i = 0;
	while ((*str)[i])
		free((*str)[i++]);
	free(*str);
	*str = NULL;
}

void				ft_astrnfree(char ***str, int n)
{
	if (!str || !(*str))
		return ;
	while (--n >= 0)
		free((*str)[n]);
	free(*str);
	*str = NULL;
}
