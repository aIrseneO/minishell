/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 18:22:24 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/07 21:34:34 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_astrsize(char **astr)
{
	int		size;

	size = 0;
	while (astr[size])
		size++;
	return (size);
}

int			ft_astrcpy(char **dest, char **src)
{
	while (*src)
		if ((*dest++ = ft_strdup(*src++)) == NULL)
			return (-1);
	return (0);
}
