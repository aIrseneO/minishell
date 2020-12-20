/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 22:59:49 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/19 23:10:00 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//gw ft_error_free_exit.c ft_parse_input.c ft_init.c main.c ft_signal_handler.c libft.a

int					main(int ac, char **av, char **env)
{
	int				n;
	pid_t			father;
	t_cmd			cmds;

	(void)ac; (void)av;
	if (signal(SIGQUIT, sigquit_ctrl_slash_handler) == SIG_ERR ||
			signal(SIGINT, sigint_ctrl_c_handler) == SIG_ERR ||
			ft_init(&cmds, env) == -1)
		return (ft_perror(strerror(errno), NULL));
	while (1)
	{
		if (write(1, "\x1B[32mMinishell $> \x1B[0m", 22) == -1 ||
				(n = get_next_line(0, &cmds.line)) == -1)
			return (ft_perror(strerror(errno), &cmds));
		if (!n && !(*cmds.line))
			ft_free_and_exit(&cmds);
		if (ft_parse_input(&cmds) == -1)
			return (ft_perror(strerror(errno), &cmds));
		n = 0;
		while (cmds.line1[n++])
		{
			if (cmds.do_fork)
			{
				if ((father = fork()) == -1)
					return (ft_perror(strerror(errno), &cmds));
				if (father > 0 && wait(NULL) == -1)
				{
					kill(SIGQUIT, father);
					return (ft_perror(strerror(errno), &cmds));
				}
				else if (father == 0)
					ft_execvp(cmds.line); //ft_execute_cmds(cmds.line, envp);
			}
			else
				write(1, "Change\n", 8); // ft_run(cmd)////////////////
		}
		ft_free_t_cmd(&cmds);
		ft_init_t_cmd(&cmds);
	}
	return (ft_perror("Oops, something went wrong!", NULL));
}
//_ https://www.gnu.org/software/libc/manual/html_node/Resource-Usage.html
//_ https://brandonwamboldt.ca/how-bash-redirection-works-under-the-hood-1512/
//_ https://brandonwamboldt.ca/how-linux-pipes-work-under-the-hood-1518/
//_ https://ideone.com/fedrB8
