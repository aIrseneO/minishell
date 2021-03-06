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

static void	sh_handle_last_child_ps_returned_value(t_data *data,
				int wstatus, int n)
{
	if (WIFEXITED(wstatus))
	{
		data->status = WEXITSTATUS(wstatus);
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

/*
** Bash doesn't display anything from intermediate pipes, this can be seen
** when running a `cat | echo` in Bash, sending SIGINT or SIGQUIT doesn't
** display anything
*/

static void	sh_wait_and_handle_intermediate_pipes_returned_values(t_data *data)
{
	while (data->ispiped >= 0)
		if (waitpid(data->children_pid[data->ispiped--], NULL, 0) == -1)
			sh_free_data_exit1(data, NULL, strerror(errno), -1);
}

static void	sh_parse_and_execute_each_cmd(t_data *data, int n)
{
	int		wstatus;

	if (signal(SIGINT, sig_handler1) == SIG_ERR ||
			signal(SIGQUIT, sig_handler2) == SIG_ERR)
		sh_free_data_exit1(data, NULL, strerror(errno), -1);
	while (data->line1[n])
	{
		if (sh_parse_cmds(data, n++))
			break ;
		if (!(!data->ispiped && data->cmd[0]->app
						&& sh_run_if_non_binary(data, n)))
		{
			sh_execute_pipes(data, STDIN_FILENO, 0);
			if (waitpid(data->children_pid[data->ispiped--], &wstatus, 0) == -1)
				sh_free_data_exit1(data, NULL, strerror(errno), -1);
			sh_wait_and_handle_intermediate_pipes_returned_values(data);
			sh_handle_last_child_ps_returned_value(data, wstatus, n);
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
	while (1)
	{
		if (signal(SIGINT, sigint_ctrl_c_handler) == SIG_ERR ||
				signal(SIGQUIT, sigquit_ctrl_slash_handler) == SIG_ERR)
			sh_free_data_exit1(&data, NULL, strerror(errno), -1);
		sh_get_command_line(&data, 0);
		data.line1 = ft_split3(data.line, ';',
						sh_isquotation, sh_isbackslash);
		if (data.line1 == NULL)
			sh_free_data_exit1(&data, NULL, strerror(errno), -1);
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
