/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_syntax_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/02 22:37:40 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/22 23:49:24 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			sh_bad_syntax(char *s, char c)
{
	write(STDERR_FILENO,
		"\x1B[33mMinishell: \x1B[0msyntax error near unexpected token `", 56);
	if (s)
		write(STDERR_FILENO, s, ft_strlen(s));
	else
		write(STDERR_FILENO, &c, 1);
	write(STDERR_FILENO, "'\n", 2);
	return (-1);
}

static int	sh_set_status_prompt_and_return(t_data *data)
{
	data->status = 2;
	prompt(data->mode);
	if (data->line)
		ft_strdel(&data->line);
	return (-1);
}

int			sh_syntax_check(t_data *data)
{
	int		ij[2];
	char	*newline;

	if (!data->line)
		return (-1);
	if ((*data->line == '|' && sh_bad_syntax(NULL, *data->line)))
		return (sh_set_status_prompt_and_return(data));
	if (sh_recursive_check(data, &data->line, 0) == -1)
		return (sh_set_status_prompt_and_return(data));
	newline = (char *)malloc(sizeof(*newline)
			* (2 * ft_strlen(data->line) + 1));
	if (newline == NULL && sh_perror_return("\x1B[31mMinishell: \x1B[0m",
			NULL, strerror(errno), -1) == -1)
		return (sh_set_status_prompt_and_return(data));
	ij[0] = 0;
	ij[1] = 0;
	newline = sh_fix_quotations(data->line, newline, ij);
	ft_strdel(&data->line);
	data->line = newline;
	return (1);
}
