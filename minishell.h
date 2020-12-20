/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 20:47:42 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/19 23:15:57 by atemfack         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <errno.h>
# include "libft.h"

# include <string.h>

# define RED "\x1B[31m"
# define GRN "\x1B[32m"
# define RESET "\x1B[0m"

typedef struct			s_cmd
{
	int			do_fork;
	char		*line;
	char		**line1;
	char		**envp;
        int				redirection;
		int				pipe;
		char			*pathname;
		char			**argv;
}						t_cmd;

int		ft_perror(char *_errmsg, t_cmd *cmds);
void	ft_free_and_exit(t_cmd *cmds);
void	ft_free_t_cmd(t_cmd *cmds);

int		ft_execute_cmds(char *line, char **envp);

void	sigint_ctrl_c_handler(int signum);
void	sigquit_ctrl_slash_handler(int signum);
void	ft_execvp(char *line); ////////////////////////////////////

int		ft_init(t_cmd *cmds, char **env);
void	ft_init_t_cmd(t_cmd *cmds);

int		ft_parse_input(t_cmd *cmds);

#endif
