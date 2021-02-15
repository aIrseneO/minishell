/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_input_utils4.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 18:05:04 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/12 04:42:38 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*sh_clean(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[j])
	{
		if (str[j] == QUOTATION || str[j] == BACKSLASH
			|| str[j] == OLDQUOTES || str[j] == NONDOLLAR)
			j++;
		else if (str[j] == DOLLARSIG && ++j)
			str[i++] = '$';
		else
			str[i++] = str[j++];
	}
	str[i] = '\0';
	return (str);
}

static char	*sh_replace(char *line, int *i, char *strmid, char *end)
{
	char	*tmp;

	if (strmid == NULL)
		return (NULL);
	line[*i - 1] = '\0';
	tmp = ft_strjoin2(line, strmid, end);
	*i += ft_strlen(strmid) - 1;
	free(strmid);
	if (tmp == NULL)
		return (NULL);
	return (tmp);
}

static char	*sh_replace_envp_value(char *line, int *i, char **envp)
{
	int		n;
	char	*tmp;

	n = 0;
	tmp = line + *i;
	while (tmp[n] && (ft_isalpha(tmp[n])
			|| tmp[n] == '_' || ft_isdigit(tmp[n])))
		n++;
	while (*envp)
	{
		if (!strncmp(tmp, *envp, n) && (*envp)[n] == '=')
			break ;
		envp++;
	}
	if (*envp == NULL)
		return (sh_replace(line, i, ft_strdup(""), tmp + n));
	return (sh_replace(line, i, ft_strdup(*envp + n + 1), tmp + n));
}

static char	*sh_replace_element(char *line, int *i, char c, t_data *data)
{
	int		digit;

	if (c == '#')
		return (sh_replace(line, i, ft_itoa(data->ac), line + *i + 1));
	if (c == '$')
		return (sh_replace(line, i, ft_itoa(data->shellpid), line + *i + 1));
	if (c == '?')
		return (sh_replace(line, i, ft_itoa(data->status), line + *i + 1));
	if (c == '_')
	{
		digit = 0;
		while (data->envp[digit] && ft_strncmp(data->envp[digit], "_=", 2))
			digit++;
		if (!data->envp[digit])
			return (sh_replace(line, i, ft_strdup("_"), line + *i + 1));
		else
			return (sh_replace_envp_value(line, i, data->envp));
	}
	digit = c - '0';
	if (digit < data->ac)
		return (sh_replace(line, i, ft_strdup(data->av[digit]), line + *i + 1));
	return (sh_replace(line, i, ft_strdup(""), line + *i + 1));
}

char	*sh_recursive_replace_dollar_clean(char *line, int i, t_data *data)
{
	char	*new;

	while (line[i] && line[i] != '$')
		i++;
	if (line[i] == '\0')
		return (sh_clean(line));
	if (line[i] == '$' && (line[i + 1] == '=' || sh_isquotation(line[i + 1])
			|| sh_isbackslash(line[i + 1]) || sh_is_back_escape(line, i - 1)))
		return (sh_recursive_replace_dollar_clean(line, i + 1, data));
	if (line[++i] == '\0')
		return (sh_clean(line));
	else if (ft_isdigit(line[i]) || ft_strchr("#$?_", line[i]))
		new = sh_replace_element(line, &i, line[i], data);
	else
		new = sh_replace_envp_value(line, &i, data->envp);
	if (new == NULL)
		return (NULL);
	ft_strdel(&line);
	return (sh_recursive_replace_dollar_clean(new, i, data));
}
