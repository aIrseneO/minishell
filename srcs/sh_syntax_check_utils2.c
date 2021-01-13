/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_syntax_check_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 21:17:18 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/12 00:27:32 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sh_fill_inner_quote(char *newline, int *i, char **line, char quote)
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
	{
		n = (*line)++ - tmp;
		ft_strlcpy(newline + *i, tmp, n + 1);
		*i = *i + n;
		while (!ft_isquotation(**line))
			newline[(*i)++] = *(*line)++;
		if (**line != quote)
			sh_fill_inner_quote(newline, i, line, quote);
	}
}

static void	sh_recursive_fill(char *newline, int *i, char **line, char quote)
{
	while (!ft_isquotation(**line))
		newline[(*i)++] = *(*line)++;
	if (**line != quote)
		sh_fill_inner_quote(newline, i, line, quote);
	(*line)++;
	while (**line && !ft_strchr(";|<>'\"", **line) && !ft_isspace(**line))
		newline[(*i)++] = *(*line)++;
	quote = ft_isquotation(**line);
	if (quote)
	{
		(*line)++;
		sh_recursive_fill(newline, i, line, quote);
	}
}

static void	sh_fill_prequote(char *newline, int *i, char **line)
{
	char		*tmp;
	int			n;
	char		quote;

	tmp = *line;
	while (*tmp && !ft_strchr(";|<>'\"", *tmp) && !ft_isspace(*tmp))
		tmp++;
	n = tmp - *line;
	quote = ft_isquotation(*tmp);
	if (quote)
	{
		newline[(*i)++] = quote;
		ft_strlcpy(newline + *i, *line, n + 1);
		*line = tmp + 1;
		*i = *i + n;
		sh_recursive_fill(newline, i, line, quote);
		newline[(*i)++] = quote;
		return ;
	}
	ft_strlcpy(newline + *i, *line, n + 1);
	*i += n;
	*line = tmp;
}

char	*sh_fix_quotations(char *line, int i)
{
	char		newline[PATH_MAX];
	char		quote;

	while (*line)
	{
		while (ft_strchr(";|<>", *line) || ft_isspace(*line))
			newline[i++] = *line++;
		quote = ft_isquotation(*line);
		if (quote)
		{
			newline[i++] = *line++;
			sh_recursive_fill(newline, &i, &line, quote);
			newline[i++] = quote;
		}
		else
			sh_fill_prequote(newline, &i, &line);
	}
	newline[i] = '\0';
	return (ft_strdup(newline));
}

/*
**		To test sh_fix_quotations
** gw sh_syntax_check_utils2.c libft.a
** int			main(int argc, char **argv)
** {
** 	if (argc != 2)
** 		return (1);
** 	ft_printf("->%s<-\n", argv[1]);
** 	ft_printf("->%s<-\n", sh_fix_quotations(argv[1], 0));
** 	return (1);
** }
*/
