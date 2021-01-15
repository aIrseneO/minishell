/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_input_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/27 23:07:07 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/14 21:49:06 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sh_load_argv(char ***argv, t_list *args)
{
	int				i;

	*argv = ft_astrinit(ft_lstsize(args) + 2);
	if (*argv == NULL)
		return (-1);
	i = 0;
	(*argv)[i] = ft_strdup("42");
	if ((*argv)[i++] == NULL)
		return (-1);
	while (args)
	{
		(*argv)[i] = ft_strdup(args->content);
		if ((*argv)[i++] == NULL)
			return (-1);
		args = args->next;
	}
	return (1);
}

static char	*sh_replace(char *str, int *i, char *strmid, char *end)
{
	char	*tmp;

	if (strmid == NULL)
		return (NULL);
	str[*i - 1] = '\0';
	tmp = ft_strjoin2(str, strmid, end);
	*i += ft_strlen(strmid) - 1;
	free(strmid);
	if (tmp == NULL)
		return (NULL);
	return (tmp);
}

static char	*sh_replace_envp_value(char *str, int *i, char **envp)
{
	int		n;
	char	*tmp;

	n = 0;
	tmp = str + *i;
	while (tmp[n] && !ft_strchr("$/", tmp[n]))
		n++;
	while (*envp)
	{
		if (!strncmp(tmp, *envp, n) && (*envp)[n] == '=')
			break ;
		envp++;
	}
	if (*envp == NULL)
		return (sh_replace(str, i, ft_strdup(""), tmp + n));
	return (sh_replace(str, i, ft_strdup(*envp + n + 1), tmp + n));
}

static char	*sh_recursively_replace(char *str, int i, char **envp, int exno)
{
	char	*new;

	if (str[i] == '\0')
		return (str);
	if (str[i] != '$')
		return (sh_recursively_replace(str, i + 1, envp, exno));
	if (str[++i] == '\0')
		return (str);
	if (str[i] == '?')
		new = sh_replace(str, &i, ft_itoa(exno), str + i + 1);
	else
		new = sh_replace_envp_value(str, &i, envp);
	if (new == NULL)
		return (NULL);
	free(str);
	return (sh_recursively_replace(new, i, envp, exno));
}

int	sh_replace_dollarsign(char **app, char ***argv, char **envp, int exno)
{
	int		i;
	char	*tmp;

	if (*app)
	{
		*app = sh_recursively_replace(*app, 0, envp, exno);
		if (*app == NULL)
			return (-1);
	}
	i = 1;
	while ((*argv)[i])
	{
		tmp = sh_recursively_replace(ft_strdup((*argv)[i]), 0, envp, exno);
		if (tmp == NULL)
			return (-1);
		free((*argv)[i]);
		(*argv)[i++] = tmp;
	}
	return (0);
}
