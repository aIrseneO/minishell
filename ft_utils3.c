/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 17:17:17 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/07 21:20:00 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*ft_strjoin2(char const *s1, char const *s2, char const *s3)
{
	size_t	i;
	size_t	len;
	char	*s;

	if (!(s1) || !(s2) || !(s3))
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	if ((s = (char *)malloc(sizeof(*s) * (len + 1))) == NULL)
		return (NULL);
	i = 0;
	while (*s1)
		s[i++] = *s1++;
	while (*s2)
		s[i++] = *s2++;
	while (*s3)
		s[i++] = *s3++;
	s[i] = '\0';
	return (s);
}

void		ft_cswap(char *c1, char *c2)
{
	char	c;

	c = *c1;
	*c1 = *c2;
	*c2 = c;
}

int			ft_isredirection(char c)
{
	if (c == '<')
		return ('<');
	if (c == '>')
		return ('>');
	return (0);
}

int			ft_isquotation(char c)
{
	if (c == '\'')
		return ('\'');
	if (c == '"')
		return ('"');
	return (0);
}
