/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 20:53:17 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/28 01:31:52 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			ft_extract_token(char **cmd, t_list **list)
{
	char			*tmp;
	t_list			*new;

	tmp = *cmd;
	if (ft_isredirection(*tmp))
		while (*tmp && ft_isredirection(*tmp))
			tmp++;
	else
		while (*tmp && !ft_isspace(*tmp) && !(ft_isredirection(*tmp)))
			tmp++;
	if ((new = ft_new_list(*cmd, tmp)) == NULL)
		return (-1);
	ft_lstadd_back(list, new);
	*cmd = tmp;
	return (1);
}

static int			ft_extract_app(char **cmd, char **app)
{
	int				n;
	char			*tmp;

	tmp = *cmd;
	while (*tmp && !ft_isspace(*tmp) && !(ft_isredirection(*tmp)))
		tmp++;
	n = tmp - *cmd;
	if ((*app = (char *)malloc(sizeof(**app) * (n + 1))) == NULL)
		return (-1);
	(*app)[n] = '\0';
	ft_strlcpy(*app, *cmd, n + 1);
	*cmd = tmp;
	return (1);
}

static int			ft_recursive_parse_cmd(char *cmd, t_cmd *scmd)
{
	cmd = ft_isfx_ptrmove(cmd, ft_isspace, NULL);
	if (!(*cmd))
		return (1);
	if (ft_isredirection(*cmd))
	{
		if (ft_extract_token(&cmd, &scmd->redirections) == -1)
			return (-1);
		cmd = ft_isfx_ptrmove(cmd, ft_isspace, NULL);
		if (!(*cmd))
			return (1);
		if (ft_extract_token(&cmd, &scmd->files) == -1)
			return (-1);
	}
	else if (scmd->app)
	{
		if (ft_extract_token(&cmd, &scmd->args) == -1)
			return (-1);
	}
	else
		if (ft_extract_app(&cmd, &scmd->app) == -1)
			return (-1);
	return (ft_recursive_parse_cmd(cmd, scmd));
}

int					ft_parse_cmds(t_data *data, int i)
{
	int				n;

	if ((data->line2 = ft_split(data->line1[i], '|')) == NULL)
		return (-1);
	n = 0;
	while (data->line2[n])
		n++;
	if (ft_init_scmd(&data->scmd, n) == -1)
		return (-1);
	while (--n >= 0)
	{
		if (ft_recursive_parse_cmd(data->line2[n], &data->scmd[n]) == -1)
			return (-1);
		ft_printf("cmd: %s\n", data->scmd[n].app);

		t_list *x = data->scmd[n].args;
		ft_printf("args: %s, ", x->content);
		while ((x = x->next))
			ft_printf("%s, ", x->content);
		write(1, "\n", 1);

		x = data->scmd[n].redirections;
		if (x){
		ft_printf("redirections: %s, ", x->content);
		while ((x = x->next))
			ft_printf("%s, ", x->content);
		write(1, "\n", 1);}
		

		x = data->scmd[n].files;
		if (x){
		ft_printf("files: %s, ", x->content);
		while ((x = x->next))
			ft_printf("%s, ", x->content);
		write(1, "\n", 1);}
	}
	return (1);
}
