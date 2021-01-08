/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 10:53:07 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/07 20:53:33 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_export_repeated_keys(t_data *data, int i, int j, int n)
{
	char	*value;

	while (data->cmd[0]->argv[j])
	{
		if (!ft_strncmp(data->cmd[0]->argv[j], "PATH=", 5) &&
				!(data->path = ft_split(data->cmd[0]->argv[j] + 5, ':')))
			return (-1);
		if ((value = ft_strchr(data->cmd[0]->argv[j], '=')))
		{
			i = 0;
			n = value - data->cmd[0]->argv[j] + 1;
			while (data->envp[i])
			{
				if (!ft_strncmp(data->cmd[0]->argv[j], data->envp[i], n))
				{
					if ((value = ft_strdup(data->cmd[0]->argv[j])) == NULL)
						return (-1);
					free(data->envp[i]);
					data->envp[i] = value;
					data->cmd[0]->argv[j][0] = '\0';
				}
				i++;
			}
		}
		j++;
	}
	return (0);
}

static int		ft_load_new_envp(char **new_envp, char **envp, char **argv)
{
	int			i;
	char		*value;

	if (ft_astrcpy(new_envp, envp) == -1)
	{
		ft_astrfree(&new_envp);
		return (-1);
	}
	i = ft_astrsize(new_envp);
	while (*argv)
	{
		if ((value = ft_strchr(*argv, '=')) &&
				!(new_envp[i++] = ft_strdup(*argv)))
		{
			ft_astrfree(&new_envp);
			return (-1);
		}
		argv++;
	}
	return (0);
}

int				ft_export(t_data *data)
{
	int			size;
	char		**new_envp;
	char		**argv;

	if (ft_export_repeated_keys(data, 0, 0, 0) == -1)
		return (ft_perror_return("\x1B[31mMinishell: \x1B[0m", 
					"export", strerror(errno), 1));
	size = 0;
	argv = data->cmd[0]->argv;
	while (*argv)
		if (ft_strchr(*argv++, '='))
			size++;
	if (!size)
		return (0);
	size += ft_astrsize(data->envp);
	if ((new_envp = ft_astrinit(size + 1)) == NULL)
		return (ft_perror_return("\x1B[31mMinishell: \x1B[0m", 
					"export", strerror(errno), 1));
	if (ft_load_new_envp(new_envp, data->envp, data->cmd[0]->argv) == -1)
		return (ft_perror_return("\x1B[31mMinishell: \x1B[0m",
                                        "export", strerror(errno), 1));
	ft_astrfree(&data->envp);
	data->envp = new_envp;
	return (0);
}
