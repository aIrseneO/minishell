/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_input_utils1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/27 23:07:07 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/13 04:32:31 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	sh_extract_redirections(char *line2, int *i, t_list **list)
{
	char	*tmp;
	t_list	*new;

	tmp = line2 + *i + 1;
	if (*tmp && ft_isredirection(*tmp))
		tmp++;
	new = ft_lstnew3(line2 + *i, tmp - 1);
	if (new == NULL)
		return (-1);
	ft_lstadd_back(list, new);
	tmp = ft_isfx_ptrmove(tmp, ft_isspace, NULL);
	*i = tmp - line2;
	return (0);
}

static int	sh_load_app(t_data *data, char **app, int isquote)
{
	*app = sh_recursive_replace_dollar_clean(*app, 0, data);
	if (*app == NULL)
		return (-1);
	if (**app == '\0' && !isquote)
		ft_strdel(app);
	else if (sh_make_absolute_path(app, data) == -1)
		return (-1);
	return (0);
}

static int	sh_extract_app(t_data *data, char *line2, int *i, char **app)
{
	char	*begin;
	char	*end;
	int		isquote;

	isquote = 0;
	if (line2[*i] == QUOTATION)
	{
		begin = line2 + ++(*i);
		while (line2[*i] != QUOTATION)
			(*i)++;
		end = line2 + (*i)++ - ++isquote;
	}
	else
	{
		begin = line2 + *i;
		while (line2[*i] && !((ft_isredirection(line2[*i])
					|| ft_isspace(line2[*i]))
				&& !sh_is_back_escape(line2, *i - 1)))
			(*i)++;
		end = line2 + *i - 1;
	}
	*app = ft_strndup(begin, end - begin + 1);
	if (*app == NULL)
		return (-1);
	return (sh_load_app(data, app, isquote));
}

int			sh_recursive_parse_line2(t_data *data, char *line2,
				int i, t_cmd *cmd)
{
	int		m;

	i = ft_isfx_ptrmove(line2 + i, ft_isspace, NULL) - line2;
	if (!line2[i])
		return (0);
	if (ft_isredirection(line2[i]) && !(sh_is_back_escape(line2, i - 1)))
	{
		if (sh_extract_redirections(line2, &i, &cmd->redirections) == -1)
			return (-1);
		m = sh_extract_file(data, line2, &i, &cmd->files);
		if (m != 0)
			return (m);
	}
	else if (cmd->app)
	{
		if (sh_extract_args(data, line2, &i, &cmd->args) == -1)
			return (-1);
	}
	else if (sh_extract_app(data, line2, &i, &cmd->app) == -1)
		return (-1);
	return (sh_recursive_parse_line2(data, line2, i, cmd));
}
