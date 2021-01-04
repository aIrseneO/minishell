/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/02 22:37:40 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/04 01:01:28 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			ft_bad_syntax(char *s, char c)
{
	write(1,
		"\x1B[33mMinishell: \x1B[0msyntax error near unexpected token `", 56);
	if (s)
		write(1, s, ft_strlen(s));
	else
		write(1, &c, 1);
	write(1, "'\n", 2);
	return (-1);
}

static int			ft_recursive_check(char **line, int i)
{
	char			*str;
	char			*tmp;

	if (!(*line)[i])
		return (1);
	if ((*line)[i] == '&' || (*line)[i] == '(' || (*line)[i] == ')')
		return (ft_bad_syntax(NULL, (*line)[i]));
	if ((*line)[i] == ';')
	{
		i += ft_isfx_ptrmove(*line + i + 1, ft_isspace, NULL) - (*line + i);
		if ((*line)[i] == ';')
			return (ft_bad_syntax(NULL, (*line)[i]));
	}
	if ((*line)[i] == '|')
	{
		if ((*line)[i + 1] == '|')
			return (ft_bad_syntax("||", 0));
		i += ft_isfx_ptrmove(*line + i + 1, ft_isspace, NULL) - (*line + i);
		if ((*line)[i] == '|' || (*line)[i] == ';')
			return (ft_bad_syntax(NULL, (*line)[i]));
		if (!(*line)[i])
		{
			while (1)
			{
				write(1, "\x1B[34m>\x1B[0m", 10);
				if (get_next_line(0, &str) == -1)
					return (ft_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
						strerror(errno), -1));
				if (*str)
					break ;
				free(str);
			}
			tmp = ft_strjoin(*line, str);
			free(str);
			free(*line);
			*line = tmp;
		}
	}
	if (ft_isredirection((*line)[i]))
	{
		if (ft_isredirection((*line)[i + 1] &&
				!ft_strncmp(*line + i++, "><", 2)))
			return (ft_bad_syntax("<", 0));
		i += ft_isfx_ptrmove(*line + i + 1, ft_isspace, NULL) - (*line + i);
		if (ft_strchr("|;><", (*line)[i]))
			return (ft_bad_syntax(NULL, (*line)[i]));
		if (!(*line)[i])
			return (ft_bad_syntax("newline", 0));
	}
	if ((*line)[i] == '\'' || (*line)[i] == '"')
	{
		char c = (*line)[i++];
		while ((*line)[i] && (*line)[i] != c)
			i++;
		if (!(*line)[i])
		{
			while (1)
			{
				write(1, "\x1B[34m>\x1B[0m", 10);
				if (get_next_line(0, &str) == -1)
					return (ft_perror_return("\x1B[31mMinishell: \x1B[0m", NULL,
						strerror(errno), -1));
				if (ft_strchr(str, c))
					break ;
				free(str);
			}
			tmp = ft_strjoin(*line, str);
			free(str);
			free(*line);
			*line = tmp;
		}
	}
	else
		i += ft_isfx_ptrmove(*line + i + 1, ft_isspace, NULL) - (*line + i);
	return (ft_recursive_check(line, i));
}

int			ft_syntax_check(char **line)
{
	if (!(*line))
		return (-1);
	if (ft_recursive_check(line, 0) == -1)
	{
		free(*line);
		*line = NULL;
		PROMPT;
		return (-1);
	}
	return (1);
}

//gw ft_syntax_check.c ft_error_free_exit.c ft_utils1.c ft_parse_input_utils.c libft.a
/*int			main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	char *s = ft_strdup(argv[1]);
	int n = ft_syntax_check(&s);
	//ft_printf("->%d<-\n", n);
	ft_printf("->%s<-\n", s);
	if (n != -1)
		free(s);
	return (1);
}*/
