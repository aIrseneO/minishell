/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_input_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 02:24:01 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/14 13:08:39 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	sh_add_back1(t_data *data, t_list **list, t_list *new, int isquote)
{
	char	*tmp;

	if (new)
		tmp = ft_strdup(new->content);
	if (new == NULL || tmp == NULL)
		return (-1);
	new->content = sh_recursive_replace_dollar_clean(new->content, 0, data);
	if (new->content == NULL)
	{
		free(tmp);
		ft_lstclear(&new, NULL);
		return (-1);
	}
	if (((char *)new->content)[0] == '\0' && !isquote)
	{
		sh_perror_return("\x1B[33mMinishell: \x1B[0m", tmp,
			"ambiguous redirect", 2);
		free(tmp);
		ft_lstclear(&new, free);
		prompt(data->mode);
		return (2);
	}
	free(tmp);
	ft_lstadd_back(list, new);
	return (0);
}

int	sh_extract_file(t_data *data, char *line2, int *i, t_list **list)
{
	char			*begin;
	char			*end;
	int				isquote;

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
	return (sh_add_back1(data, list, ft_lstnew3(begin, end), isquote));
}

static int	sh_add_back2(t_data *data, t_list **list, t_list *new, int isquote)
{
	if (new == NULL)
		return (-1);
	new->content = sh_recursive_replace_dollar_clean(new->content, 0, data);
	if (new->content == NULL)
	{
		ft_lstclear(&new, NULL);
		return (-1);
	}
	if (((char *)new->content)[0] == '\0' && !isquote)
		ft_lstclear(&new, free);
	else
		ft_lstadd_back(list, new);
	return (0);
}

int	sh_extract_args(t_data *data, char *line2, int *i, t_list **list)
{
	char			*begin;
	char			*end;
	int				isquote;

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
	return (sh_add_back2(data, list, ft_lstnew3(begin, end), isquote));
}
