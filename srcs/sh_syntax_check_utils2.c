/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_syntax_check_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 13:29:51 by atemfack          #+#    #+#             */
/*   Updated: 2021/03/10 23:21:59 by atemfack         ###   ########.fr       */
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

static int	sh_get_line_quotation(t_data *data, char **newline, int n, char *tmp1)
{
	n = get_next_line(data->fd, newline);
	if (n == -1)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
	if (n == 0)
	{
		if (!(**newline))
		{
			free(*newline);
			free(tmp1);
			sh_perror_return("\x1B[33mMinishell: \x1B[0m", NULL,
				"unexpected EOF while looking for matching `\"'", -1);
			return (sh_perror_return("\x1B[33mMinishell: \x1B[0m",
				"syntax error", "unexpected end of file", -1));
		}
		sigappend_ctrl_d_handler(data, newline, n);
	}
	return (0);
}

static int	sh_get_line_to_append(t_data *data, char c, char **newline, int *i)
{
	char	*tmp;
	char	*tmp1;
	char	*tmp2;

	while (1)
	{
		prompt2();
		tmp1 = *newline;
		if (sh_get_line_quotation(data, newline, 0, tmp1) == -1)
			return (-1);
		tmp2 = *newline;
		*newline = ft_strjoin2(tmp1, "\n", tmp2);
		free(tmp1);
		tmp = ft_strchr(tmp2, c);
		if (tmp && *newline && !sh_is_back_escape(tmp2, tmp - tmp2 - 1))
			break ;
		*i += ft_strlen(tmp2) + 1;
		free(tmp2);
		if (*newline == NULL)
			sh_free_data_exit1(data, NULL, strerror(errno), X);
	}
	*i += tmp - tmp2 + 2;
	free(tmp2);
	return (0);
}

static void	sh_update_line(t_data *data, char *newline, int *i, int flag)
{
	char	*tmp;

	if (flag)
	{
		data->line[flag] = '\0';
		tmp = ft_strjoin2(NULL, data->line, newline + 1);
		*i -= 2;
	}
	else	
		tmp = ft_strjoin2(NULL, data->line, newline);
	free(newline);
	if (!tmp)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
	free(data->line);
	data->line = tmp;
}

/*
** The flag is for the case: 
** Minishell_> echo "\
** > no new line will be output"
** no new line will be output
*/

int			sh_check_quotation(t_data *data, char **line, int i, char c)
{
	int		flag;
	char	*newline;

	while ((*line)[i] && !((*line)[i] == c
				&& !(sh_is_back_escape(*line, i - 1) && c != '\'')))
		i++;
	if (!(*line)[i])
	{
		flag = 0;
		if (i > 1 && c == '"' && (*line)[i - 1] == '\\')
			flag = i - 1;
		newline = NULL;
		if (sh_get_line_to_append(data, c, &newline, &i) == -1)
			return (-1);	
		sh_update_line(data, newline, &i, flag);
	}
	else
		i++;
	return (sh_recursive_check(data, line, i));
}
