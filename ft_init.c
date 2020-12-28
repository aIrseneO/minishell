/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 12:21:22 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/26 16:35:13 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_init_t_data(t_data *data)
{
	data->line = NULL;
	data->line1 = NULL;
	data->line2 = NULL;
	//data-> = NULL;
}

static int	ft_load_path_envp(char *tmp1, t_envp *envp)
{
	char	*tmp2;
	char	*tmp3;
	int		n;

	tmp2 = NULL;
	tmp3 = NULL;
	if (!(tmp2 = ft_strjoin("PATH=", tmp1)) || 
			!(tmp3 = ft_strjoin(tmp2, "/bin")) ||
			!(envp->next = ft_envpnew(tmp3)))
		n = -1;
	else
		n = 1;
	free(tmp1);
	free(tmp2);
	free(tmp3);
	return (n);
}

static int	ft_load_pwd_envp(t_envp *envp)
{
	char	*tmp1;
	char	*tmp2;

	if (!(tmp1 = ft_getcwd()))
		return (-1);
	tmp2 = NULL;
	if (!(tmp2 = ft_strjoin("PWD=", tmp1)) || !(envp->next = ft_envpnew(tmp2)))
	{
		free(tmp1);
		free(tmp2);
		return (-1);
	}
	free(tmp2);
	return (ft_load_path_envp(tmp1, envp->next));
}

static int	ft_load_envp(char **env, t_envp **envp)
{
	t_envp	*user;
	t_envp	*home;

	user = NULL;
	home = NULL;
	while (*env)
	{
 		if ((!ft_strncmp(*env, "USER=", 5) && !(user = ft_envpnew(*env))) ||
				(!ft_strncmp(*env, "HOME=", 5) && !(home = ft_envpnew(*env))))
		{
			ft_envpdelone(user, ft_astrfree);
			ft_envpdelone(home, ft_astrfree);
			return (-1);
		}
		env++;
	}
	if ((!user && !(user = ft_envpnew(USER))) ||
		(!home && !(home = ft_envpnew(HOME))) || !(*envp = ft_envpnew("?=0")))
	{
		ft_envpdelone(user, ft_astrfree);
		ft_envpdelone(home, ft_astrfree);
		return (-1);
	}
	(*envp)->next = user;
	return (ft_load_pwd_envp(user->next = home));
}

int			ft_init(t_data *data, char **env)
{
	ft_init_t_data(data);
	data->envp = NULL;
	if (ft_load_envp(env, &data->envp) == -1)
		return (ft_perror(strerror(errno), data));
	return (1);
}
