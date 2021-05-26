/*
* Copyright (c) 2020 Arsene Temfack
*
* SPDX-License-Identifier: MIT
*/

#include "minishell.h"

static void	sh_get_line_to_append_pipe(t_data *data, char **line, int n)
{
	while (1)
	{
		prompt2();
		n = get_next_line(data->fd, line);
		if (n == -1)
			sh_free_data_exit1(data, NULL, strerror(errno), X);
		else if (n == 0)
		{
			if (!(**line))
				sigexit_ctrl_d_handler(data, sh_perror_return(
					"\x1B[33Minishell: \x1B[0m", "syntax error",
						"unexpected end of file", 2));
			sigappend_ctrl_d_handler(data, line, n);
		}
		else if (**line && ft_strcmp(*line, "\\"))
			break ;
		free(*line);
	}
}

static int	sh_check_pipe(t_data *data, char **line, int i)
{
	char	*newline;
	char	*tmp;

	if ((*line)[i + 1] == '|')
		return (sh_bad_syntax("||", 0));
	i += ft_isfx_ptrmove(*line + i + 1, ft_isspace, NULL) - (*line + i);
	if (((*line)[i] == '|' || (*line)[i] == ';'))
		return (sh_bad_syntax(NULL, (*line)[i]));
	if (!(*line)[i] || !ft_strcmp((*line) + 1, "\\"))
	{
		sh_get_line_to_append_pipe(data, &newline, 0);
		tmp = ft_strjoin(*line, newline);
		free(newline);
		if (!tmp)
		{
			return (sh_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
					strerror(errno), -1));
		}
		free(*line);
		*line = tmp;
	}
	return (sh_recursive_check(data, line, i));
}

static int	sh_check_escape(t_data *data, char **line, int i, int n)
{
	char	*tmp;
	char	*newline;

	while ((*line)[i] && (*line)[i] == '\\')
	{
		n++;
		i++;
	}
	if ((*line)[i] && n % 2)
		return (sh_recursive_check(data, line, i + 1));
	if (!(*line)[i] && n % 2)
	{
		(*line)[--i] = '\0';
		prompt2();
		if (get_next_line(data->fd, &newline) == -1)
			sh_free_data_exit1(data, NULL, strerror(errno), X);
		tmp = ft_strjoin(*line, newline);
		free(newline);
		if (!tmp)
			sh_free_data_exit1(data, NULL, strerror(errno), X);
		free(*line);
		*line = tmp;
	}
	return (sh_recursive_check(data, line, i));
}

static int	sh_check_redirection(t_data *data, char **line, int i)
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

/*
** TODO: Implement `&()` (& -> for jobs)
*/

int			sh_recursive_check(t_data *data, char **line, int i)
{
	if (!(*line)[i])
		return (1);
	if ((*line)[i] == '\\')
		return (sh_check_escape(data, line, i, 0));
	if (ft_strchr("&()", (*line)[i]))
		return (sh_bad_syntax(NULL, (*line)[i]));
	if ((*line)[i] == '|')
		return (sh_check_pipe(data, line, i));
	if (ft_isredirection((*line)[i]))
		return (sh_check_redirection(data, line, i));
	if (ft_isquotation((*line)[i]))
		return (sh_check_quotation(data, line, i + 1, (*line)[i]));
	if ((*line)[i] == ';')
		return (sh_check_semicolon(data, line, i));
	return (sh_recursive_check(data, line, i + 1));
}
