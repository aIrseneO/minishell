/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_syntax_check_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 21:17:18 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/11 05:30:16 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sh_fill_postquote(char *line, char *newline, int ij[2]);

static void	sh_fill1(char *line, char *newline, int ij[2])
{
	while (line[ij[1]] != '"')
	{
		if (line[ij[1]] == '\\')
		{
			if (ft_strchr("`\"\\", line[ij[1] + 1]) && ij[1]++)
				newline[(ij[0])++] = line[(ij[1])++];
			else if (line[ij[1] + 1] == '$')
			{
				newline[(ij[0])++] = DOLLARSIG;
				ij[1] += 2;
			}
			else
				newline[(ij[0])++] = line[(ij[1])++];
		}
		else
			newline[(ij[0])++] = line[(ij[1])++];
	}
	newline[ij[0]++] = OLDQUOTES;
	ij[1]++;
	sh_fill_postquote(line, newline, ij);
}

static void	sh_fill2(char *line, char *newline, int ij[2])
{
	while (line[ij[1]] != '\'')
	{
		if (line[ij[1]] == '$' && ij[1]++)
			newline[(ij[0])++] = DOLLARSIG;
		else
			newline[(ij[0])++] = line[(ij[1])++];
	}
	newline[ij[0]++] = OLDQUOTES;
	ij[1]++;
	sh_fill_postquote(line, newline, ij);
}

static void	sh_fill_postquote(char *line, char *newline, int ij[2])
{
	char		*tmp;
	int			n;
	char		quote;

	n = ij[1];
	tmp = line + ij[1];
	while (line[ij[1]] && !((ft_strchr(";|<>'\"", line[ij[1]])
				|| ft_isspace(line[ij[1]]))
			&& !sh_is_back_escape(line, ij[1] - 1)))
		ij[1]++;
	n = ij[1] - n;
	if (ft_isquotation(line[ij[1]]))
	{
		sh_memcpy(newline + ij[0], tmp, n);
		ij[0] += n;
		newline[(ij[0])++] = OLDQUOTES;
		quote = line[ij[1]++];
		if (quote == '"')
			sh_fill1(line, newline, ij);
		else
			sh_fill2(line, newline, ij);
		return ;
	}
	sh_memcpy(newline + ij[0], tmp, n);
	ij[0] += n;
}

static void	sh_fill_prequote(char *line, char *newline, int ij[2])
{
	char		*tmp;
	int			n;

	n = ij[1];
	tmp = line + ij[1];
	while (line[ij[1]] && !((ft_strchr(";|<>'\"", line[ij[1]])
				|| ft_isspace(line[ij[1]]))
			&& !sh_is_back_escape(line, ij[1] - 1)))
		ij[1]++;
	n = ij[1] - n;
	if (ft_isquotation(line[ij[1]]))
	{
		newline[(ij[0])++] = QUOTATION;
		sh_memcpy(newline + ij[0], tmp, n);
		ij[0] += n;
		newline[(ij[0])++] = OLDQUOTES;
		if (line[ij[1]++] == '"')
			sh_fill1(line, newline, ij);
		else
			sh_fill2(line, newline, ij);
		newline[(ij[0])++] = QUOTATION;
		return ;
	}
	sh_memcpy(newline + ij[0], tmp, n);
	ij[0] += n;
}

// TODO: Find a better way to implement this withous using non printable chars.
// Outer quotations surrounding tokens will be replace by QUOTATION which is 
// 3 END OF TEXT (ETX), non printed back-slash will be replaced by
// BACKSLASH which is 8	BACKSPACE (BS), printed dollar signes will be repaced by
// 1 and non printed by 4, and old quotations will be replaced by 2.
// sh_putstr_fd prints those chars in color.
// It's done this way to preserve infos about the command line and fix
// quotations at the same time. Saved infos are needed to clear back-slash 
// characters and replace strings preceded by dollar sign `$HOME` ... .

char	*sh_fix_quotations(char *line, char *newline, int ij[2])
{
	char		quote;

	while (line[ij[1]])
	{
		while (line[ij[1]] && (ft_strchr(";|<>", line[ij[1]])
				|| ft_isspace(line[ij[1]])))
			newline[ij[0]++] = line[ij[1]++];
		if (ft_isquotation(line[ij[1]]) && !sh_is_back_escape(line, ij[1] - 1))
		{
			quote = line[ij[1]++];
			newline[ij[0]++] = QUOTATION;
			newline[ij[0]++] = OLDQUOTES;
			if (quote == '"')
				sh_fill1(line, newline, ij);
			else
				sh_fill2(line, newline, ij);
			newline[ij[0]++] = QUOTATION;
		}
		else
			sh_fill_prequote(line, newline, ij);
	}
	newline[ij[0]] = '\0';
	//sh_putstr_fd(newline, STDERR_FILENO);  //////
	return (newline);
}
