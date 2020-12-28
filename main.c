/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 22:59:49 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/28 00:42:37 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//gw ft_error_free.c ft_parse_input.c ft_parse_input_utils.c ft_init.c main.c ft_signal_handler.c ft_utils1.c ft_envp_utils.c ft_execute.c ft_init_utils.c libft.a
void					ft_prompt(void)
{
	write(1, "\x1B[32mMinishell $> \x1B[0m", 22);
}

int					main(int ac, char **av, char **env)
{
	int				n;
	int				wstatus;
	pid_t			father;
	t_data			data;


	(void)ac; (void)av;
	if (signal(SIGQUIT, sigquit_ctrl_slash_handler) == SIG_ERR ||
			signal(SIGINT, sigint_ctrl_c_handler) == SIG_ERR ||
			ft_init(&data, env) == -1)
		return (ft_perror(strerror(errno), NULL));
	ft_prompt();
	while (1)
	{
		if ((n = get_next_line(0, &data.line)) == -1)
			return (ft_perror(strerror(errno), &data));
		if (!n && !(*data.line))
			sigexit_ctrl_d_handler(&data);
		if ((data.line1 = ft_split(data.line, ';')) == NULL)
			return (ft_perror(strerror(errno), &data));
		n = 0;
		if (!data.line1[0])
			ft_prompt();
		while (data.line1[n])
		{
			if (ft_parse_cmds(&data, n++) == -1)
				return (ft_perror(strerror(errno), &data));
			if (!data.line2[1] && ft_isfathercmd(data.scmd->app))
			{
				write(1, "Change\n", 8); // TODO change ft_run(cmd)
				ft_prompt();
			}
			else
			{
				if ((father = fork()) == -1)
					return (ft_perror(strerror(errno), &data));
				if (father == 0)
					ft_execute_recursive_pipe(&data, STDIN_FILENO, 0);
				if (wait(&wstatus) == -1)
					return (ft_perror(strerror(errno), &data));
				if (WIFEXITED(wstatus))
				{
					if ((data.status = WEXITSTATUS(wstatus)) == -1)
						write(1, "\x1B[31mFailled\x1B[0m\n", 17);
					if (!data.line1[n])
						ft_prompt();
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
						ft_prompt();
					}
				}
			}
			ft_astrfree(&data.line2);///////////////
		}
		ft_free_t_data(&data);
		ft_init_t_data(&data);
	}
	return (ft_perror("Oops, something went wrong!", NULL));
}
//_ https://www.gnu.org/software/libc/manual/html_node/Resource-Usage.html
//_ https://brandonwamboldt.ca/how-bash-redirection-works-under-the-hood-1512/
//_ https://brandonwamboldt.ca/how-linux-pipes-work-under-the-hood-1518/
//_ https://ideone.com/fedrB8
