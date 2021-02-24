/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 22:59:49 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/23 04:57:18 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sh_handle_child_ps_returned_value(t_data *data, int wstatus, int n)
{
	if (wstatus == -1)
		sh_perror_exit(NULL, NULL, "Child poccess faillure", -1);
	if (WIFEXITED(wstatus))
	{
		data->status = WEXITSTATUS(wstatus);
		if (data->status == -1)
			write(STDERR_FILENO, "\x1B[31mFailled\x1B[0m\n", 17);
		if (!data->line1[n])
			prompt(data->mode);
	}
	else if (WIFSIGNALED(wstatus))
	{
		if (WTERMSIG(wstatus) == SIGINT)
			data->status = 130;
		else if (WTERMSIG(wstatus) == SIGQUIT)
			data->status = 131;
		if (WCOREDUMP(wstatus))
		{
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
			prompt(data->mode);
		}
	}
}

static void	sh_parse_and_execute_each_cmd(t_data *data, int n, int wstatus)
{
	pid_t	father;

	while (data->line1[n])
	{
		wstatus = sh_parse_cmds(data, n++, 0);
		if (wstatus == -1)
			exit(sh_perror_free_t_data(strerror(errno), data));
		if (wstatus == 2)
			break ;
		if (!data->line2[1] && data->cmd[0]->app)
			wstatus = sh_run_if_father_app(data, n);
		if (wstatus == -1)
			sh_perror_exit(RED, NULL, strerror(errno), -1);
		if (wstatus == 0)
		{
			father = fork();
			if (father == -1)
				exit(sh_perror_free_t_data(strerror(errno), data));
			if (father == 0)
				sh_execute_recursive_pipe(data, STDIN_FILENO, 0, 0);
			if (wait(&wstatus) == -1)
				exit(sh_perror_free_t_data(strerror(errno), data));
			sh_handle_child_ps_returned_value(data, wstatus, n);
		}
	}
}

static void	sh_minishell(t_data *data, int n)
{
	while (1)
	{
		while (sh_syntax_check(data) == -1)
		{
			n = get_next_line(data->fd, &data->line);
			if (n == -1)
				exit(sh_perror_free_t_data(strerror(errno), data));
			if (n == 0)
			{
				if (!(*data->line))
					sigexit_ctrl_d_handler(data, data->status);
				sigappend_ctrl_d_handler(data, &data->line, n);
			}
		}
		data->line1 = ft_split3(data->line, ';',
						sh_isquotation, sh_isbackslash);
		if (data->line1 == NULL)
			exit(sh_perror_free_t_data(strerror(errno), data));
		if (!data->line1[0])
			prompt(data->mode);
		sh_parse_and_execute_each_cmd(data, 0, 0);
		sh_reset_t_data(data);
	}
}

/*
** After Initialization set: data.mode += !(isatty(data.fd)) * 2;
*/

int			main(int ac, char **av, char **env)
{
	t_data	data;

	if (signal(SIGQUIT, sigquit_ctrl_slash_handler) == SIG_ERR
		|| signal(SIGINT, sigint_ctrl_c_handler) == SIG_ERR)
		return (sh_perror_free_t_data(strerror(errno), NULL));
	if (sh_init(&data, ac, av, env) == -1)
		return (sh_perror_free_t_data(strerror(errno), &data));
	prompt(data.mode);
	sh_minishell(&data, 0);
	return (sh_perror_free_t_data("Oops, something went wrong!", NULL));
}
/*
**		Resources:
**_ https://www.gnu.org/software/libc/manual/html_node/Resource-Usage.html
**_ https://brandonwamboldt.ca/how-bash-redirection-works-under-the-hood-1512/
**_ https://brandonwamboldt.ca/how-linux-pipes-work-under-the-hood-1518/
**_ https://ideone.com/fedrB8
*/
