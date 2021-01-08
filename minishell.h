/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 20:47:42 by atemfack          #+#    #+#             */
/*   Updated: 2021/01/08 02:08:09 by atemfack         ###   ########.fr       */
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
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>

# include "libft.h"

# include <string.h>

# define RED		"\x1B[31m"
# define GRN		"\x1B[32m"
# define YEL		"\x1B[33m"
# define BLU		"\x1B[34m"
# define MAG		"\x1B[35m"
# define CYN		"\x1B[36m"
# define WHT		"\x1B[37m"
# define RESET		"\x1B[0m"
# define PATH_MAX	4096
# define USER		"USER=user42"
# define HOME		"HOME=/home/user42"
# define PROMPT		write(1, "\x1B[32mMinishell_> \x1B[0m", 21)

#define TEST1 write(1, "test1\n", 7); //////
#define TEST2 write(1, "test2\n", 7); //////
#define TEST3 write(1, "test3\n", 7); //////

/*
** Formats layour:
** line1:	line1[0]                             ; line1[1]   ...    ; line1[[n]
** line2:	line2[0] | line2[1] | ... | line2[n] ;
** cmd:		{cmd[0] ,  cmd[1] , ... ,  cmd[n]}
*/

typedef struct			s_cmd
{
	char				*app;
	t_list				*args;
	t_list				*redirections;
	t_list				*files;
	char				**argv;
}							t_cmd;

typedef struct			s_data
{
	char		*pwd; /////
	char		**path; /////
	char		**envp;

	char		*line;
	char		**line1;
	char		**line2;
	t_cmd		**cmd;
	int			status;
}						t_data;

int		ft_perror(char *_errmsg, t_data *data);
void	ft_free_t_data(t_data *data);
void	ft_cmdfree(t_cmd ***cmd);
void	ft_perror_exit(char *s1, char *s2, char *s3, int n);
int		ft_perror_return(char *s1, char *s2, char *s3, int n);

void	ft_execute_recursive_pipe(t_data data, int fd, int i);
void	ft_execute_recursive_redirection(t_data data, int i);
void	ft_execve(t_data data, int i);
int		ft_execute_if_father_app(t_data *data);

int		ft_export(t_data *data);
int		ft_unset(t_data *data);
void	ft_exit(t_data data);

void	sigint_ctrl_c_handler(int signum);
void	sigquit_ctrl_slash_handler(int signum);
void	sigexit_ctrl_d_handler(t_data *data);

int		ft_init(t_data *data, char **env);
int		ft_init_cmd(t_cmd ***cmd, int n);
void	ft_reset_t_data(t_data *data);

char	**ft_astrinit(int size);
void	ft_astrprint(char **astr);
int		ft_astrcpy(char **dest, char **src);
void	ft_astrfree(char ***astr);
void	ft_astrnfree(char ***astr, int n);
int		ft_astrsize(char **astr);
char	*ft_getcwd(void);
void	ft_cswap(char *c1, char *c2);
int		ft_isquotation(char c);
int		ft_isredirection(char c);
char	*ft_strjoin2(char const *s1, char const *s2, char const *s3);

int		ft_parse_cmds(t_data *data, int i);
int		ft_load_argv(char ***argv, t_list *agrs);
t_list	*ft_new_list(char *begin, char *end);

int		ft_replace_envp_in_cmd(char **app, char ***argv, char **envp);

int		ft_syntax_check(char **line);
int		ft_bad_syntax(char *s, char c);
int		ft_recursive_check(char **line, int i);
char	*ft_fix_quotations(char *line, int i);

#endif
