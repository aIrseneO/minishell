/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_syntax_check_utils1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 21:21:55 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/11 05:15:51 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	sh_get_line_to_append(char c, char **line, char **tmp, int empty)
{
	while (1)
	{
		write(STDERR_FILENO, "\x1B[34m>\x1B[0m", 10);
		if (get_next_line(0, line) == -1)
			return (sh_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
					strerror(errno), -1));
		if (tmp)
		{
			*tmp = ft_strchr(*line, c);
			if (*tmp)
				break ;
		}
		else if (empty)
			break ;
		else if (**line && ft_strcmp(*line, "\\"))
			break ;
		free(*line);
	}
	return (1);
}

static int	sh_check_pipe(char **line, int i)
{
	char			*newline;
	char			*tmp;

	if ((*line)[i + 1] == '|')
		return (sh_bad_syntax("||", 0));
	i += ft_isfx_ptrmove(*line + i + 1, ft_isspace, NULL) - (*line + i);
	if (((*line)[i] == '|' || (*line)[i] == ';'))
		return (sh_bad_syntax(NULL, (*line)[i]));
	if (!(*line)[i] || !ft_strcmp((*line) + 1, "\\"))
	{
		if (sh_get_line_to_append('\0', &newline, NULL, 0) == -1)
			return (-1);
		tmp = ft_strjoin(*line, newline);
		free(newline);
		if (!tmp)
			return (sh_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
					strerror(errno), -1));
		free(*line);
		*line = tmp;
	}
	return (sh_recursive_check(line, i));
}

static int	sh_check_quotation(char **line, int i, char c)
{
	char			*tmp;
	char			*newline;

	while ((*line)[i] && !((*line)[i] == c
				&& !(sh_is_back_escape(*line, i - 1) && c != '\'')))
		i++;
	if (!(*line)[i])
	{
		if (sh_get_line_to_append(c, &newline, &tmp, 0) == -1)
			return (-1);
		i += tmp - newline + 2;
		tmp = ft_strjoin2(*line, "\n", newline);
		free(newline);
		if (!tmp)
			return (sh_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
					strerror(errno), -1));
		free(*line);
		*line = tmp;
	}
	else
		i++;
	return (sh_recursive_check(line, i));
}

static int	sh_check_escape(char **line, int i, int n)
{
	char	*tmp;
	char	*newline;

	while ((*line)[i] && (*line)[i] == '\\')
	{
		n++;
		i++;
	}
	if ((*line)[i] && n % 2)
		return (sh_recursive_check(line, i + 1));
	if (!(*line)[i] && n % 2)
	{
		(*line)[--i] = '\0';
		if (sh_get_line_to_append('\0', &newline, NULL, 1) == -1)
			return (-1);
		tmp = ft_strjoin(*line, newline);
		free(newline);
		if (!tmp)
			return (sh_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
					strerror(errno), -1));
		free(*line);
		*line = tmp;
	}
	return (sh_recursive_check(line, i));
}

int	sh_recursive_check(char **line, int i)
{
	if (!(*line)[i])
		return (1);
	if ((*line)[i] == '\\')
		return (sh_check_escape(line, i, 0));
	if (ft_strchr("&()", (*line)[i])) // TODO: Implement
		return (sh_bad_syntax(NULL, (*line)[i]));
	if ((*line)[i] == '|')
		return (sh_check_pipe(line, i));
	if (ft_isredirection((*line)[i]))
		return (sh_check_redirection(line, i));
	if (ft_isquotation((*line)[i]))
		return (sh_check_quotation(line, i + 1, (*line)[i]));
	if ((*line)[i] == ';')
		return (sh_check_semicolon(line, i));
	return (sh_recursive_check(line, i + 1));
}
