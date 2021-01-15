/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 20:53:17 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/14 21:28:59 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	sh_extract_redirections(char **line2, t_list **list)
{
	char			*tmp;
	t_list			*new;

	tmp = *line2 + 1;
	if (*tmp && ft_isredirection(*tmp))
		tmp++;
	new = ft_lstnew3(*line2, tmp - 1);
	if (new == NULL)
		return (-1);
	ft_lstadd_back(list, new);
	tmp = ft_isfx_ptrmove(tmp, ft_isspace, NULL);
	*line2 = tmp;
	return (1);
}

static int	sh_extract_token(char **line2, t_list **list)
{
	char			*begin;
	char			*end;
	t_list			*new;

	if (ft_isquotation(**line2))
	{
		begin = *line2 + 1;
		end = begin;
		while (*end != **line2)
			end++;
		*line2 = end-- + 1;
	}
	else
	{
		begin = *line2;
		end = begin;
		while (*end && !ft_isspace(*end) && !(ft_isredirection(*end)))
			end++;
		*line2 = end--;
	}
	new = ft_lstnew3(begin, end);
	if (new == NULL)
		return (-1);
	ft_lstadd_back(list, new);
	return (1);
}

static int	sh_extract_app(char **line2, char **app)
{
	int				n;
	char			*tmp;

	tmp = *line2;
	while (*tmp && !ft_isspace(*tmp) && !(ft_isredirection(*tmp)))
		tmp++;
	n = tmp - *line2;
	*app = (char *)malloc(sizeof(**app) * (n + 1));
	if (*app == NULL)
		return (-1);
	(*app)[n] = '\0';
	ft_strlcpy(*app, *line2, n + 1);
	*line2 = tmp;
	return (1);
}

static int	sh_recursive_parse_line2(char *line2, t_cmd *cmd)
{
	line2 = ft_isfx_ptrmove(line2, ft_isspace, NULL);
	if (!(*line2))
		return (1);
	if (ft_isredirection(*line2))
	{
		if (sh_extract_redirections(&line2, &cmd->redirections) == -1
			|| sh_extract_token(&line2, &cmd->files) == -1)
			return (-1);
	}
	else if (cmd->app)
	{
		if (sh_extract_token(&line2, &cmd->args) == -1)
			return (-1);
	}
	else if (sh_extract_app(&line2, &cmd->app) == -1)
		return (-1);
	return (sh_recursive_parse_line2(line2, cmd));
}

int	sh_parse_cmds(t_data *data, int i)
{
	int				n;

	ft_astrfree(&data->line2, free);
	data->line2 = ft_split(data->line1[i], '|');
	if (data->line2 == NULL)
		return (-1);
	n = 0;
	while (data->line2[n])
		n++;
	if (sh_init_cmd(&data->cmd, n) == -1)
		return (-1);
	while (--n >= 0)
	{
		if (sh_recursive_parse_line2(data->line2[n], data->cmd[n]) == -1)
			return (-1);
		if (sh_load_argv(&data->cmd[n]->argv, data->cmd[n]->args) == -1)
			return (-1);
		if (sh_replace_dollarsign(&data->cmd[n]->app, &data->cmd[n]->argv,
				data->envp, data->status) == -1)
			return (-1);
	}
	return (1);
}
