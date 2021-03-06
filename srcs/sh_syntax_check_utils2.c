/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_syntax_check_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 13:29:51 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/23 05:32:58 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			sh_check_semicolon(t_data *data, char **line, int i)
{
	i += ft_isfx_ptrmove(*line + i + 1, ft_isspace, NULL) - (*line + i);
	if ((*line)[i] == ';' || (*line)[i] == '|')
		return (sh_bad_syntax(NULL, (*line)[i]));
	return (sh_recursive_check(data, line, i));
}

int			sh_check_redirection(t_data *data, char **line, int i)
{
	if (ft_isredirection((*line)[i + 1])
		&& !ft_strncmp(*line + i++, "><", 2))
		return (sh_bad_syntax("<", 0));
	i += ft_isfx_ptrmove(*line + i + 1, ft_isspace, NULL) - (*line + i);
	if (!(*line)[i])
		return (sh_bad_syntax("newline", 0));
	if (ft_strchr("|;><", (*line)[i]))
		return (sh_bad_syntax(NULL, (*line)[i]));
	return (sh_recursive_check(data, line, i));
}

static int	sh_get_line_quotation(t_data *data, char **line, int n, char *tmp1)
{
	n = get_next_line(data->fd, line);
	if (n == -1)
		sh_free_data_exit1(data, NULL, strerror(errno), -1);
	if (n == 0)
	{
		if (!(**line))
		{
			free(*line);
			free(tmp1);
			sh_perror_return("\x1B[33mMinishell: \x1B[0m", NULL,
				"unexpected EOF while looking for matching `\"'", -1);
			return (sh_perror_return("\x1B[33mMinishell: \x1B[0m",
				"syntax error", "unexpected end of file", -1));
		}
		sigappend_ctrl_d_handler(data, line, n);
	}
	return (0);
}

static int	sh_get_line_to_append(t_data *data, char c, char **line, int *i)
{
	char	*tmp;
	char	*tmp1;
	char	*tmp2;

	while (1)
	{
		prompt2();
		tmp1 = *line;
		if (sh_get_line_quotation(data, line, 0, tmp1) == -1)
			return (-1);
		tmp2 = *line;
		*line = ft_strjoin2(tmp1, "\n", tmp2);
		free(tmp1);
		tmp = ft_strchr(tmp2, c);
		if (tmp && *line && !sh_is_back_escape(tmp2, tmp - tmp2 - 1))
			break ;
		*i += ft_strlen(tmp2) + 1;
		free(tmp2);
		if (*line == NULL)
			sh_free_data_exit1(data, NULL, strerror(errno), -1);
	}
	*i += tmp - tmp2 + 2;
	free(tmp2);
	return (0);
}

int			sh_check_quotation(t_data *data, char **line, int i, char c)
{
	char	*tmp;
	char	*newline;

	while ((*line)[i] && !((*line)[i] == c
				&& !(sh_is_back_escape(*line, i - 1) && c != '\'')))
		i++;
	if (!(*line)[i])
	{
		newline = NULL;
		if (sh_get_line_to_append(data, c, &newline, &i) == -1)
			return (-1);
		tmp = ft_strjoin2(NULL, *line, newline);
		free(newline);
		if (!tmp)
			sh_free_data_exit1(data, NULL, strerror(errno), -1);
		free(*line);
		*line = tmp;
	}
	else
		i++;
	return (sh_recursive_check(data, line, i));
}
