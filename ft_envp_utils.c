/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envp_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 21:37:19 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/28 01:17:38 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envp		*ft_envpnew(char *content)
{
	t_envp	*envp;
	char	*s;
	int		n;

	if ((s = ft_strchr(content, '=')) == NULL || 
			(envp = (t_envp *)malloc(sizeof(*envp))) == NULL)
		return (NULL);
	n = s - content;
	envp->key = NULL;
	if ((envp->key = (char *)malloc(sizeof(*envp->key) * (n + 1))) == NULL ||
		(envp->values = ft_split(s + 1, ':')) == NULL)
	{
		free(envp->key);
		free(envp);
		return (NULL);
	}
	ft_strlcpy(envp->key, content, n + 1);
	envp->key[n] = '\0';
	envp->next = NULL;
	return (envp);
}

void		ft_envpadd_back(t_envp **envp, t_envp *nw)
{
	t_envp	*tmp;

	if (!(*envp))
	{
		*envp = nw;
		return ;
	}
	tmp = *envp;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = nw;
}

void		ft_envpclear(t_envp **envp, void (*del)(char ***))
{
	t_envp	*tmp;

	while(*envp)
	{
		tmp = (*envp)->next;
		free((*envp)->key);
		del(&(*envp)->values);
		free(*envp);
		*envp = tmp;
	}
}

void		ft_envpdelone(t_envp *envp, void (*del)(char ***))
{
	if (!envp)
		return ;
	free(envp->key);
	del(&envp->values);
	free(envp);
}
