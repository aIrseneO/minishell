/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_init_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 01:18:23 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/12 03:12:33 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	sh_load(char **str, char *s)
{
	*str = ft_strdup(s);
	if (*str == NULL)
		return (-1);
	return (0);
}

int	sh_load_envp(char **env, char **envp, char **pwd)
{
	while (*env)
	{
		if ((!ft_strncmp(*env, "USER=", 5) && sh_load(&envp[0], *env) == -1)
			|| (!ft_strncmp(*env, "HOME=", 5) && sh_load(&envp[1], *env) == -1))
			return (-1);
		env++;
	}
	*pwd = ft_getcwd();
	envp[2] = ft_strjoin("PWD=", *pwd);
	envp[3] = ft_strjoin2("PATH=", *pwd, "/bin");
	if ((*pwd == NULL)
		|| (!envp[0] && sh_load(&envp[0], USER) == -1)
		|| (!envp[1] && sh_load(&envp[1], HOME) == -1)
		|| envp[2] == NULL || envp[3] == NULL)
		return (-1);
	return (0);
}
