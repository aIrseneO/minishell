/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_syntax_check_utils1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 21:21:55 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/23 00:03:23 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sh_get_line_to_append_pipe(t_data *data, char **line, int n)
{
	while (1)
	{
		prompt2();
		n = get_next_line(data->fd, line);
		if (n == -1)
			exit(sh_perror_free_t_data(strerror(errno), data));
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
	char			*newline;
	char			*tmp;

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
			return (sh_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
					strerror(errno), -1));
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
			exit(sh_perror_free_t_data(strerror(errno), data));
		tmp = ft_strjoin(*line, newline);
		free(newline);
		if (!tmp)
			exit(sh_perror_free_t_data(strerror(errno), data));
		free(*line);
		*line = tmp;
	}
	return (sh_recursive_check(data, line, i));
}

// TODO: Implement "&()". & -> for jobs

int	sh_recursive_check(t_data *data, char **line, int i)
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
