/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_check_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 21:17:18 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/06 21:46:15 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		ft_fill_inner_quote(char *newline, int *i,
					char **line, char quote);

static void		ft_copy_inner(char *newline, int *i, char **line,
					char quote, char *tmp)
{
	int			n;

	n = (*line)++ - tmp;
	ft_strlcpy(newline + *i, tmp, n + 1);
	*i = *i + n;
	while (!ft_isquotation(**line))
		newline[(*i)++] = *(*line)++;
	if (**line != quote)
	{
		ft_fill_inner_quote(newline, i, line, quote);
	}
}

static void		ft_fill_inner_quote(char *newline, int *i,
					char **line, char quote)
{
	char		*tmp;
	int			n;

	tmp = (*line)++;
	while (!ft_isquotation(**line))
		(*line)++;
	if (**line == quote)
	{
		n = *line - tmp;
		ft_strlcpy(newline + *i, tmp, n + 1);
		*i = *i + n;
	}
	else
		ft_copy_inner(newline, i, line, quote, tmp + 1);
}

static void		ft_recursive_fill(char *newline, int *i,
					char **line, char quote)
{
	while (!ft_isquotation(**line))
		newline[(*i)++] = *(*line)++;
	if (**line != quote)
		ft_fill_inner_quote(newline, i, line, quote);
	(*line)++;
	while (**line && !ft_strchr(";|<>'\"", **line) && !ft_isspace(**line))
		newline[(*i)++] = *(*line)++;
	if ((quote = ft_isquotation(**line)))
	{
		(*line)++;
		ft_recursive_fill(newline, i, line, quote);
	}
}

static void		ft_fill_prequote(char *newline, int *i, char **line)
{
	char		*tmp;
	int			n;
	char		quote;

	tmp = *line;
	while (*tmp && !ft_strchr(";|<>'\"", *tmp) && !ft_isspace(*tmp))
		tmp++;
	n = tmp - *line;
	if ((quote = ft_isquotation(*tmp)))
	{
		newline[(*i)++] = quote;
		ft_strlcpy(newline + *i, *line, n + 1);
		*line = tmp + 1;
		*i = *i + n;
		ft_recursive_fill(newline, i, line, quote);
		newline[(*i)++] = quote;
		return ;
	}
	ft_strlcpy(newline + *i, *line, n + 1);
	*i += n;
	*line = tmp;
}

char			*ft_fix_quotations(char *line, int i)
{
	char		newline[PATH_MAX];
	char		quote;

	while (*line)
	{
		while (ft_strchr(";|<>", *line) || ft_isspace(*line))
			newline[i++] = *line++;
		if ((quote = ft_isquotation(*line)))
		{
			newline[i++] = *line++;
			ft_recursive_fill(newline, &i, &line, quote);
			newline[i++] = quote;
		}
		else
			ft_fill_prequote(newline, &i, &line);
	}
	newline[i] = '\0';
	return (ft_strdup(newline));
}

/*
**		To test ft_fix_quotations
** gw ft_syntax_check_utils2.c ft_utils1.c ft_utils2.c libft.a
** int			main(int argc, char **argv)
** {
** 	if (argc != 2)
** 		return (1);
** 	ft_printf("->%s<-\n", argv[1]);
** 	ft_printf("->%s<-\n", ft_fix_quotations(argv[1], 0));
** 	return (1);
** }
*/
