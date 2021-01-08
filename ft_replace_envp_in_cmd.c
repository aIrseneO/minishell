/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replace_envp_in_cmd.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 02:07:01 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/08 03:03:41 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_replace(char **s, char **envp, char *key)
{
	int		n;

	n = ft_strlen(key);
	if (!n)
		return (0);
	while (*envp)
		if (!ft_strncmp(*envp++, key, n))
			break ;
	if (!(*envp))
	{
		*(key - 1) = '\0';
		return (0);
	}
	return (0);
	(void)s;
}

int	ft_replace_envp_in_cmd(char **app, char ***argv, char **envp)
{
	char	*tmp;

	tmp = ft_strchr(*app, '$');
	if (tmp && ft_replace(app, envp, tmp + 1) == -1)
		return (ft_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
				strerror(errno), -1));
	while (**argv)
	{
		tmp = ft_strchr(**argv, '$');
		if (tmp && ft_replace(*argv, envp, tmp + 1) == -1)
			return (ft_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
					strerror(errno), -1));
		(**argv)++;
	}
	return (0);
}
