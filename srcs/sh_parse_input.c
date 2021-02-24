/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 20:53:17 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/13 04:22:20 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	sh_load_argv(char ***argv, t_list *args)
{
	int		i;

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
	return (0);
}

static int	sh_update_envp_(t_data *data)
{
	int		i;
	char	*env;

	i = 1;
	while (data->cmd[0]->argv[i])
		i++;
	if (i > 1)
		env = ft_strjoin("_=", data->cmd[0]->argv[i - 1]);
	else if (data->cmd[0]->app)
		env = ft_strjoin("_=", data->cmd[0]->app);
	else
		env = ft_strdup("_=");
	if (env == NULL)
		return (-1);
	i = sh_export_str(data, env, ft_strchr(env, '=') - env);
	free(env);
	if (i == -1)
		return (-1);
	return (0);
}

int			sh_parse_cmds(t_data *data, int i, int m)
{
	int		j;

	ft_astrfree(&data->line2, free);
	sh_cmdfree(&data->cmd);
	data->line2 = ft_split3(data->line1[i], '|',
					sh_isquotation, sh_isbackslash);
	if (data->line2 == NULL)
		return (-1);
	if (sh_init_cmd(&data->cmd, ft_astrsize(data->line2)) == -1)
		return (-1);
	j = 0;
	while (data->line2[j])
	{
		m = sh_recursive_parse_line2(data, data->line2[j], 0, data->cmd[j]);
		if (m != 0)
			return (m);
		if (sh_load_argv(&data->cmd[j]->argv, data->cmd[j]->args) == -1)
			return (-1);
		j++;
	}
	if (j == 1 && sh_update_envp_(data) == -1)
		return (-1);
	return (0);
}
