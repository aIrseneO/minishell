/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_syntax_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/02 22:37:40 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/12 00:03:14 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sh_bad_syntax(char *s, char c)
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

int	sh_syntax_check(char **line)
{
	char			*newline;

	if (!(*line))
		return (-1);
	if (sh_recursive_check(line, 0) == -1)
	{
		free(*line);
		*line = NULL;
		PROMPT;
		return (-1);
	}
	newline = sh_fix_quotations(*line, 0);
	free(*line);
	*line = NULL;
	*line = newline;
	if (*line == NULL)
	{
		sh_perror_return("\x1B[31nMinishell: \x1B[0m", NULL,
				strerror(errno), -1);
		PROMPT;
		return (-1);
	}
	return (1);
}

/*
**			To test sh_syntax_check function
** gw sh_syntax_check.c sh_error_free_exit.c sh_utils1.c sh_parse_input_utils.c libft.a
** int			main(int argc, char **argv)
** {
** 	if (argc != 2)
** 		return (1);
** 	char *s = ft_strdup(argv[1]);
** 	int n = sh_syntax_check(&s);
** 	//ft_printf("->%d<-\n", n);
** 	ft_printf("->%s<-\n", s);
** 	if (n != -1)
** 		free(s);
** 	return (1);
** }
*/
