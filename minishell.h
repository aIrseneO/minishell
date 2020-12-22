/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 20:47:42 by atemfack          #+#    #+#             */
/*   Updated: 2020/12/22 01:40:38 by atemfack         ###   ########.fr       */
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

# define RED		"\x1B[31m"
# define GRN		"\x1B[32m"
# define RESET		"\x1B[0m"
# define PATH_MAX	4096
# define USER		"user42"
# define HOME		"/home/user42"

#define P(x) ft_printf("%s\n", x);
#define TEST1 write(1, "test1\n", 7);
#define TEST2 write(1, "test2\n", 7);
#define TEST3 write(1, "test3\n", 7);

/*
** Formats:
** line:	line1[i0] [[;] line1[i1] [;] line1[i2] [;] ...]
** line1:	line2[i0] [[|] line2[i1] [|] line2[i2] [|] ...]
** line2:	
*/
typedef struct			s_cmd
{
	char		**envp;
	
	int			do_fork;
	int			status;
	char		*line;
	char		**line1;
	char		**line2;
}						t_cmd;

int		ft_perror(char *_errmsg, t_cmd *cmds);
void	ft_free_t_cmd(t_cmd *cmds);

int		ft_execute_cmds(char *line, char **envp);

void	sigint_ctrl_c_handler(int signum);
void	sigquit_ctrl_slash_handler(int signum);
void	sigexit_ctrl_d_handler(t_cmd *cmds);
void	ft_execvp(char *line); ////////////////////////////////////

int		ft_init(t_cmd *cmds, char **env);
void	ft_init_t_cmd(t_cmd *cmds);

char	**ft_strinit(int size);
void	ft_strfree(char ***str);
void	ft_strnfree(char ***str, int n);
char	*ft_getcwd(void);

int		ft_parse_input(t_cmd *cmds);



#endif
