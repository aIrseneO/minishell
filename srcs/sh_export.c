/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 21:48:46 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/13 05:10:08 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	sh_update_env_var(char *argv, t_data *data)
{
	if (!ft_strncmp(argv, "PATH=", 5))
	{
		ft_astrfree(&data->path, free);
		data->path = ft_split(argv + 5, ':');
		if (data->path == NULL)
			return (-1);
	}
	else if (!ft_strncmp(argv, "PWD=", 4))
	{
		free(data->pwd_env);
		data->pwd_env = ft_strdup(argv + 4);
		if (data->pwd_env == NULL)
			return (-1);
		data->ispwd_env = 1;
	}
	else if (!ft_strncmp(argv, "OLDPWD=", 7))
	{
		free(data->oldpwd_env);
		data->oldpwd_env = ft_strdup(argv + 7);
		if (data->oldpwd_env == NULL)
			return (-1);
		data->isoldpwd_env = 1;
	}
	return (0);
}

static void	sh_display_envp(char **envp)
{
	char	*tmp;

	if (!envp)
		return ;
	while (*envp)
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		tmp = ft_strchr(*envp, '=');
		write(STDOUT_FILENO, *envp, tmp++ - *envp);
		write(STDOUT_FILENO, "=\"", 2);
		ft_putstr_fd(tmp, STDOUT_FILENO);
		ft_putendl_fd("\"", STDOUT_FILENO);
		envp++;
	}
}

static int	sh_add_and_export(t_data *data, char *str, int n)
{
	int		i;
	int		m;
	char	*tmp;

	m = n - 1;
	i = 0;
	while (data->envp[i] && !(!ft_strncmp(data->envp[i], str, m)
			&& data->envp[i][m] == '='))
		i++;
	if (!data->envp[i])
	{
		i = m - 1;
		while (str[++i + 1])
			str[i] = str[i + 1];
		str[i] = str[i + 1];
		return (sh_export_str(data, str, m));
	}
	tmp = ft_strjoin(data->envp[i], str + n + 1);
	if (tmp == NULL)
		return (-1);
	m = sh_export_str(data, tmp, m);
	free(tmp);
	return (m);
}

static int	sh_check_and_export_str(t_data *data, char *str)
{
	int		i;
	int		n;
	char	*tmp;

	tmp = ft_strchr(str, '=');
	n = tmp - str;
	if (n == 0 || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (2);
	i = 0;
	while (++i < n)
		if (!(ft_isalpha(str[i]) || ft_isdigit(str[i]) || str[i] == '_'))
			break ;
	if (i == n - 1 && str[i] == '+')
		return (sh_add_and_export(data, str, n));
	else if (i == n)
		return (sh_export_str(data, str, n));
	return (2);
}

int	sh_export(t_data *data, int i)
{
	char	**argv;
	int		n;

	argv = data->cmd[i]->argv + 1;
	if (!(*argv))
		sh_display_envp(data->envp);
	n = 0;
	while (*argv)
	{
		i = sh_check_and_export_str(data, *argv);
		if (i == 2)
		{
			n = 1;
			ft_putstr_fd("\x1B[33mMinishell: \x1B[0mexport: `", STDERR_FILENO);
			ft_putstr_fd(*argv, STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		}
		else if (i == -1)
			return (sh_perror_return("\x1B[31mMinishell: \x1B[0m", "export",
					strerror(errno), -1));
		if (sh_update_env_var(*argv++, data) == -1)
			return (sh_perror_return("\x1B[31mMinishell: \x1B[0m", "export",
					strerror(errno), -1));
	}
	return (n);
}
