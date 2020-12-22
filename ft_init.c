/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 12:21:22 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/22 01:40:29 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_init_t_cmd(t_cmd *cmds)
{
	cmds->line = NULL;
	cmds->line1 = NULL;
	cmds->line2 = NULL;

	//cmds-> = NULL;
}

static int	ft_load_envp(char **env, char **envp)
{
	while (*env)
	{
 		if ((!ft_strncmp(*env, "USER=", 5) && !(envp[1] = ft_strdup(*env))) ||
				(!ft_strncmp(*env, "HOME=", 5) && !(envp[2] = ft_strdup(*env))))
			return (-1);
		env++;
	}
	if ((!(envp[0] = ft_strdup("?=0")) || !(envp[3] = ft_getcwd()) ||
			(!envp[1] && !(envp[1] = ft_strjoin("USER=", USER))) ||
			(!envp[2] && !(envp[2] = ft_strjoin("HOME=", HOME))) ||
			!(envp[5] = ft_strjoin("PATH=", envp[3])) ||
			!(envp[4] = ft_strjoin("PWD=", envp[3]))))
		return (-1);
	free(envp[3]);
	envp[3] = ft_strjoin(envp[5], "/bin");
	free(envp[5]);
	envp[5] = NULL;
	if (!(envp[3]))
		return (-1);
	return (1);
}

int			ft_init(t_cmd *cmds, char **env)
{
	int		nbr_envp;

	nbr_envp = 5;
	if (!(cmds->envp = ft_strinit(nbr_envp + 1)))
		return (-1);
	ft_init_t_cmd(cmds);
	if (ft_load_envp(env, cmds->envp) == -1)
	{
		ft_strnfree(&cmds->envp, nbr_envp + 1);
		ft_free_t_cmd(cmds);
		return (-1);
	}
	return (1);
}
