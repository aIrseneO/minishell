/******************************************************************************/
/*     Copyright (c) 2020 Arsene Temfack                                      */
/*                                                                            */
/*     SPDX-License-Identifier: MIT                                           */
/******************************************************************************/

#include "minishell.h"

static void	sh_handle_last_child_ps_returned_value(t_data *data,
				int wstatus, int *n)
{
	if (WIFEXITED(wstatus))
		data->status = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
	{
		if (WTERMSIG(wstatus) == SIGINT)
		{
			data->status = 130;
			write(STDERR_FILENO, "\n", 1);
			while (data->line1[*n])
				(*n)++;
		}
		else if (WTERMSIG(wstatus) == SIGQUIT)
			data->status = 131;
		if (WCOREDUMP(wstatus))
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
	}
	if (!data->line1[*n])
		prompt(data->mode);
}

/*
** In:`cat | echo`, `cat` is run in the intermediate process.
*/

static void	sh_wait_and_handle_intermediate_pipes_returned_values(t_data *data)
{
	int		wstatus;

	if (signal(SIGINT, sig_handler2) == SIG_ERR ||
			signal(SIGQUIT, sig_handler2) == SIG_ERR)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
	while (data->ispiped >= 0)
	{
		if (waitpid(data->children_pid[data->ispiped--], &wstatus, 0) == -1)
			sh_free_data_exit1(data, NULL, strerror(errno), X);
		if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == X)
			sh_free_data_exit1(data, NULL, strerror(errno), X);
	}
}

static void	sh_parse_and_execute_each_cmd(t_data *data, int n)
{
	int		wstatus;

	if (signal(SIGINT, sig_handler1) == SIG_ERR ||
			signal(SIGQUIT, sig_handler1) == SIG_ERR)
		sh_free_data_exit1(data, NULL, strerror(errno), X);
	while (data->line1[n])
	{
		if (sh_parse_cmds(data, n++))
			break ;
		if (!(!data->ispiped && data->cmd[0]->app
						&& sh_run_if_non_binary(data, n)))
		{
			sh_execute_pipes(data, STDIN_FILENO, 0);
			if (waitpid(data->children_pid[data->ispiped--], &wstatus, 0) == -1)
				sh_free_data_exit1(data, NULL, strerror(errno), X);
			sh_wait_and_handle_intermediate_pipes_returned_values(data);
			sh_handle_last_child_ps_returned_value(data, wstatus, &n);
		}
	}
}

static void	sh_get_command_line(t_data *data, int n)
{
	while (sh_syntax_check(data) == -1)
	{
		n = get_next_line(data->fd, &data->line);
		if (n == -1)
			sh_free_data_exit1(data, NULL, strerror(errno), X);
		if (n == 0)
		{
			if (!(*data->line))
				sigexit_ctrl_d_handler(data, data->status);
			sigappend_ctrl_d_handler(data, &data->line, n);
		}
	}
}

/*
** After Initialization set: (`isatty` isn't allow for this project)
** data.mode += !(isatty(data.fd)) * 2;
*/

int			main(int ac, char **av, char **env)
{
	t_data	data;

	sh_init(&data, ac, av, env);
	prompt(data.mode);
	g_dataptr = &data;
	while (1)
	{
		if (signal(SIGINT, sigint_ctrl_c_handler) == SIG_ERR ||
				signal(SIGQUIT, sigquit_ctrl_slash_handler) == SIG_ERR)
			sh_free_data_exit1(&data, NULL, strerror(errno), X);
		sh_get_command_line(&data, 0);
		data.line1 = ft_split3(data.line, ';',
						sh_isquotation, sh_isbackslash);
		if (data.line1 == NULL)
			sh_free_data_exit1(&data, NULL, strerror(errno), X);
		if (!data.line1[0])
			prompt(data.mode);
		else
			sh_parse_and_execute_each_cmd(&data, 0);
		sh_reset_t_data(&data);
	}
	sh_free_t_data(&data);
	return (sh_perror_return("Oops, something went wrong!", NULL,
		strerror(errno), -1));
}

/*
**		Resources:
**_ https://www.gnu.org/software/libc/manual/html_node/Resource-Usage.html
**_ https://brandonwamboldt.ca/how-bash-redirection-works-under-the-hood-1512/
**_ https://brandonwamboldt.ca/how-linux-pipes-work-under-the-hood-1518/
**_ https://ideone.com/fedrB8
*/
