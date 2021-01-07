/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_check_utils1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 21:21:55 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/05 21:19:23 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			ft_get_line_to_append(char c, char **line, char **tmp)
{
	while (1)
	{
		write(1, "\x1B[34m>\x1B[0m", 10);
		if (get_next_line(0, line) == -1)
			return (ft_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
				strerror(errno), -1));
		if (tmp)
		{
			if ((*tmp = ft_strchr(*line, c)))
				break ;
		}
		else if (**line)
			break ;
		free(*line);
	}
	return (1);
}

static int			ft_check_pipe(char **line, int *i)
{
	char			*newline;
	char			*tmp;

	if ((*line)[*i + 1] == '|')
		return (ft_bad_syntax("||", 0));
	*i += ft_isfx_ptrmove(*line + *i + 1, ft_isspace, NULL) - (*line + *i);
	if ((*line)[*i] == '|' || (*line)[*i] == ';')
		return (ft_bad_syntax(NULL, (*line)[*i]));
	if (!(*line)[*i])
	{
		if (ft_get_line_to_append('\0', &newline, NULL) == -1)
			return (-1);
		tmp = ft_strjoin(*line, newline);
		free(newline);
		if (!tmp)
			return (ft_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
				strerror(errno), -1));
		free(*line);
		*line = tmp;
	}
	return (ft_recursive_check(line, *i));
}

static int			ft_check_redirection(char *line, int *i)
{
	if (ft_isredirection(line[*i + 1]) &&
			!ft_strncmp(line + (*i)++, "><", 2))
		return (ft_bad_syntax("<", 0));
	*i += ft_isfx_ptrmove(line + *i + 1, ft_isspace, NULL) - (line + *i);
	if (!line[*i])
		return (ft_bad_syntax("newline", 0));
	if (ft_strchr("|;><", line[*i]))
		return (ft_bad_syntax(NULL, line[*i]));
	return (ft_recursive_check(&line, *i));
}

static int			ft_check_quotation(char **line, int *i, char c)
{
	char			*tmp;
	char			*newline;

	(*i)++;
	while ((*line)[*i] && (*line)[*i] != c)
		(*i)++;
	if (!(*line)[*i])
	{
		if (ft_get_line_to_append(c, &newline, &tmp) == -1)
			return (-1);
		*i += tmp - newline + 2;
		tmp = ft_strjoin2(*line, "\n", newline);
		free(newline);
		if (!tmp)
			return (ft_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
				strerror(errno), -1));
		free(*line);
		*line = tmp;
	}
	else
		(*i)++;
	return (ft_recursive_check(line, *i));
}

int					ft_recursive_check(char **line, int i)
{
	if (!(*line)[i])
		return (1);
	if (ft_strchr( "&()\\", (*line)[i]))
		return (ft_bad_syntax(NULL, (*line)[i]));
	if ((*line)[i] == ';')
	{
		i += ft_isfx_ptrmove(*line + i + 1, ft_isspace, NULL) - (*line + i);
		if ((*line)[i] == ';')
			return (ft_bad_syntax(NULL, (*line)[i]));
	}
	if ((*line)[i] == '|')
		return (ft_check_pipe(line, &i));
	if (ft_isredirection((*line)[i]))
		return(ft_check_redirection(*line, &i));
	if ((*line)[i] == '\'' || (*line)[i] == '"')
		return (ft_check_quotation(line, &i, (*line)[i]));
	i += ft_isfx_ptrmove(*line + i + 1, ft_isspace, NULL) - (*line + i);
	return (ft_recursive_check(line, i));
}
