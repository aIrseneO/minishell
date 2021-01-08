/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 10:52:07 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/07 23:34:01 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_iskey_in_envp(char *arg, char **envp, int n)
{
	while (*envp)
	{
		if (!ft_strncmp(arg, *envp, n))
		{
			**envp = '\0';
			return (1);
		}
		envp++;
	}
	return (0);
}

static int	ft_load_new_envp(char **new_envp, char **envp)
{
	int		i;

	i = 0;
	while (*envp)
	{
		if (**envp && !(new_envp[i++] = ft_strdup(*envp)))
		{
			ft_astrfree(&new_envp);
			return (-1);
		}
		envp++;
	}
	return (0);
}

int			ft_unset(t_data *data)
{
	int		n;
	char	**argv;
	char	**new_envp;

	argv = data->cmd[0]->argv;
	n = 0;
	while (*argv)
	{
		if (!ft_strcmp(*argv, "PATH"))
			ft_astrfree(&data->path);
		if (ft_iskey_in_envp(*argv, data->envp, ft_strlen(*argv)))
			n++;
		argv++;
	}
	if (n == 0)
		return (0);
	if ((new_envp = ft_astrinit(ft_astrsize(data->envp) - n + 1)) == NULL)
		return (ft_perror_return("\x1B[31mMinishell: \x1B[0m", 
					"unset", strerror(errno), 1));
	if ((ft_load_new_envp(new_envp, data->envp)) == -1)
		return (ft_perror_return("\x1B[31mMinishell: \x1B[0m", 
					"unset", strerror(errno), 1));
	ft_astrfree(&data->envp);
	data->envp = new_envp;
	return (0);
}
