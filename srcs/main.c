/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 22:59:49 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/13 19:57:52 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//gw -I libft -I includes srcs/sh_error_free_exit.c srcs/sh_parse_input.c srcs/sh_parse_input_utils.c srcs/sh_init.c srcs/sh_init_utils.c srcs/main.c srcs/sh_signal_handler.c srcs/sh_execute.c srcs/sh_execute_utils.c srcs/sh_syntax_check.c srcs/sh_syntax_check_utils1.c srcs/sh_syntax_check_utils2.c srcs/sh_run.c srcs/sh_run_unset.c srcs/sh_run_export.c libft/libft.a

int					main(int ac, char **av, char **env)
{
	int				n;
	int				wstatus;
	pid_t			father;
	t_data			data;


	(void)ac; (void)av;
	if (signal(SIGQUIT, sigquit_ctrl_slash_handler) == SIG_ERR ||
			signal(SIGINT, sigint_ctrl_c_handler) == SIG_ERR ||
			sh_init(&data, env) == -1)
		return (sh_perror_free_t_data(strerror(errno), NULL));
	PROMPT;
	while (1)
	{
		while (sh_syntax_check(&data.line) == -1)
			if ((n = get_next_line(0, &data.line)) == -1)
				return (sh_perror_free_t_data(strerror(errno), &data));
		if (!n && !(*data.line))
			sigexit_ctrl_d_handler(&data);
		if ((data.line1 = ft_split(data.line, ';')) == NULL)
			return (sh_perror_free_t_data(strerror(errno), &data));
		n = 0;
		if (!data.line1[0])
			PROMPT;
		while (data.line1[n])
		{
			if (sh_parse_cmds(&data, n++) == -1)
				return (sh_perror_free_t_data(strerror(errno), &data));
			if (!(!data.line2[1] && sh_run_if_father_app(&data, n)))
			{
				if ((father = fork()) == -1)
					return (sh_perror_free_t_data(strerror(errno), &data));
				if (father == 0)
					sh_execute_recursive_pipe(data, STDIN_FILENO, 0);
				if (wait(&wstatus) == -1)
					return (sh_perror_free_t_data(strerror(errno), &data));
				if (WIFEXITED(wstatus))
				{
					if ((data.status = WEXITSTATUS(wstatus)) == -1)
						write(1, "\x1B[31mFailled\x1B[0m\n", 17);
					if (!data.line1[n])
						PROMPT;
				}
				else if (WIFSIGNALED(wstatus))
				{
					if (WTERMSIG(wstatus) == SIGINT)
						data.status = 130;
					else if (WTERMSIG(wstatus) == SIGQUIT)
						data.status = 131;
					if (WCOREDUMP(wstatus))
					{
						write(1, "Quit (core dumped)\n", 20);
						PROMPT;
					}
				}
			}
		}
		sh_reset_t_data(&data);
	}
	return (sh_perror_free_t_data("Oops, something went wrong!", NULL));
}
//_ https://www.gnu.org/software/libc/manual/html_node/Resource-Usage.html
//_ https://brandonwamboldt.ca/how-bash-redirection-works-under-the-hood-1512/
//_ https://brandonwamboldt.ca/how-linux-pipes-work-under-the-hood-1518/
//_ https://ideone.com/fedrB8
