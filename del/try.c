/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:33:10 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/21 20:16:57 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	ft_execute(char *line, char **av, char **env)
{
	(void)av; (void)env;
	char *ag[] = {"/bin/sh", "-c", line, NULL};
	if (!ft_strcmp(line, "exit"))
		exit(1);
	execvp(ag[0], ag);

}

void	sigint_ctrl_c_handler(int signum)
{
	int	wstatus;

	(void)signum;
	if (wait(&wstatus) == -1)
		write(1, "\n\x1B[32mShell _> \x1B[0m ", 19);
	else if (WIFSIGNALED(wstatus))
		write(1, "\n", 1);
}

void	sigquit_ctrl_slash_handler(int signum)
{
	int wstatus;

	(void)signum;
	if (wait(&wstatus) != -1 && WIFSIGNALED(wstatus) && WCOREDUMP(wstatus))
		write(1, "Quit (core dumped)\n", 20);
}

int 	main(int ac, char **av, char **env)
{
	(void)ac; int n;
	char *line; pid_t father_pid;
	
	signal(SIGQUIT, sigquit_ctrl_slash_handler);
	signal(SIGINT, sigint_ctrl_c_handler);
	while (1)
	{
		write(1, "\x1B[32mShell _> \x1B[0m", 18);
		n = get_next_line(0, &line);
		if ((!n && !(*line)) || !ft_strcmp(line, "exit"))
			exit(EXIT_SUCCESS);
		father_pid = fork();
		if (father_pid == 0)
			ft_execute(line, av, env);
		else
			wait(NULL);
	}
	return (1);
}		
