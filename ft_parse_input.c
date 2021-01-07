/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 20:53:17 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/05 12:45:16 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			ft_extract_redirections(char **line2, t_list **list)
{
	char			*tmp;
	t_list			*new;

	tmp = *line2 + 1;
	if (*tmp && ft_isredirection(*tmp))
		tmp++;
	if ((new = ft_new_list(*line2, tmp)) == NULL)
		return (-1);
	ft_lstadd_back(list, new);
	tmp = ft_isfx_ptrmove(tmp, ft_isspace, NULL);
	*line2 = tmp;
	return (1);
}

static int			ft_extract_token(char **line2, t_list **list)
{
	char			*begin;
	char			*end;
	t_list			*new;
	char			c;

	if ((c = ft_isquotation(**line2)))
	{
		begin = *line2 + 1;
		end = begin;
		while (*end != c)
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
	if ((new = ft_new_list(begin, end)) == NULL)
		return (-1);
	ft_lstadd_back(list, new);
	return (1);
}

static int			ft_extract_app(char **line2, char **app)
{
	int				n;
	char			*tmp;

	tmp = *line2;
	while (*tmp && !ft_isspace(*tmp) && !(ft_isredirection(*tmp)))
		tmp++;
	n = tmp - *line2;
	if ((*app = (char *)malloc(sizeof(**app) * (n + 1))) == NULL)
		return (-1);
	(*app)[n] = '\0';
	ft_strlcpy(*app, *line2, n + 1);
	*line2 = tmp;
	return (1);
}

static int			ft_recursive_parse_line2(char *line2, t_cmd *cmd)
{
	line2 = ft_isfx_ptrmove(line2, ft_isspace, NULL);
	if (!(*line2))
		return (1);
	if (ft_isredirection(*line2))
	{
		if (ft_extract_redirections(&line2, &cmd->redirections) == -1 ||
				ft_extract_token(&line2, &cmd->files) == -1)
			return (-1);
	}
	else if (cmd->app)
	{
		if (ft_extract_token(&line2, &cmd->args) == -1)
			return (-1);
	}
	else if (ft_extract_app(&line2, &cmd->app) == -1)
			return (-1);
	return (ft_recursive_parse_line2(line2, cmd));
}

int					ft_parse_cmds(t_data *data, int i)
{
	int				n;
	
	ft_astrfree(&data->line2);
	if ((data->line2 = ft_split(data->line1[i], '|')) == NULL)
		return (-1);
	n = 0;
	while (data->line2[n])
		n++;
	if (ft_init_cmd(&data->cmd, n) == -1)
		return (-1);
	while (--n >= 0)
	{
		if (ft_recursive_parse_line2(data->line2[n], data->cmd[n]) == -1)
			return (-1);
		if (ft_load_argv(&data->cmd[n]->argv, data->cmd[n]->args) == -1)
			return (-1);
		///////////////////////////////////////////
		/*ft_printf("cmd: %s\n", data->cmd[n]->app);

		t_list *x = data->cmd[n]->args;
		if (x){
		ft_printf("args: %s, ", x->content);
		while ((x = x->next))
			ft_printf("%s, ", x->content);
		write(1, "\n", 1);}

		x = data->cmd[n]->redirections;
		if (x){
		ft_printf("redirections: %s, ", x->content);
		while ((x = x->next))
			ft_printf("%s, ", x->content);
		write(1, "\n", 1);}
		

		x = data->cmd[n]->files;
		if (x){
		ft_printf("files: %s, ", x->content);
		while ((x = x->next))
			ft_printf("%s, ", x->content);
		write(1, "\n", 1);}*/
		////////////////////////////////////////////
	}
	return (1);
}
