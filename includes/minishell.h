/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 20:47:42 by atemfack          #+#    #+#             */
/*   Updated: 2021/02/15 02:26:12 by atemfack         ###   ########.fr       */
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
# include <string.h>
# include "libft.h"

# define RED		"\x1B[31m"
# define GRN		"\x1B[32m"
# define YEL		"\x1B[33m"
# define BLU		"\x1B[34m"
# define MAG		"\x1B[35m"
# define CYN		"\x1B[36m"
# define WHT		"\x1B[37m"
# define RESET		"\x1B[0m"
# define PATH_MAX	4096
# define PATH_ENV	"PATH=/home/facko/42/minishell/bin"
# define QUOTATION	3
# define BACKSLASH	8 // To be removed
# define DOLLARSIG	1 // To stay
# define OLDQUOTES	2 // To be removed
# define NONDOLLAR	4 // To be removed

//	For debugging
//# define TEST1 write(1, "test1\n", 7); //////
//# define TEST2 write(1, "test2\n", 7); //////
//# define TEST3 write(1, "test3\n", 7); //////

//	Data layour:
//	line1:	line1[0]                             ; line1[1]   ...    ; line1[[n]
//	line2:	line2[0] | line2[1] | ... | line2[n] ;
//	cmd:	{cmd[0]	 ,  cmd[1]	, ... ,  cmd[n]}

typedef struct s_cmd
{
	char		*app;
	t_list		*args;
	t_list		*redirections;
	t_list		*files;
	char		**argv;
}	t_cmd;

//Mode: [Interactive] - 0; [With an arg (-c flag used)] - 1;
//		[With a file script] - 3; [Through pipe] - m + 2 (m is 0/1/3)

typedef struct s_data
{
	int			ac;
	char		**av;
	pid_t		shellpid; //Need getpid function, Used for $$ (current pid ps)
	int			fd;
	int			mode;
	char		*pwd;
	char		*pwd_env;
	int			ispwd_env;
	char		*oldpwd_env;
	int			isoldpwd_env;
	char		**path;
	char		**envp;
	t_list		**envpl;
	char		*line;
	char		**line1;
	char		**line2;
	t_cmd		**cmd;
	int			status;
}	t_data;

int		sh_get_line(t_data *data, int n);

void	prompt(int mode);
int		sh_isbackslash(char c);
int		sh_isquotation(char c);
void	sh_putstr_fd(char *s, int fd);

int		sh_perror_free_t_data(char *_errmsg, t_data *data);
void	sh_free_t_data(t_data *data);
void	sh_cmdfree(t_cmd ***cmd);
void	sh_perror_exit(char *s1, char *s2, char *s3, int n);
int		sh_perror_return(char *s1, char *s2, char *s3, int n);

void	sh_execute_recursive_pipe(t_data *data, int fd, int i, pid_t father);
void	sh_recursive_redirection(t_data *data, int i, int (*exc)(t_data*, int));
int		sh_execve(t_data *data, int i);

int		sh_run_if_father_app(t_data *data, int i);
void	sh_execute_if_father_app(t_data *data, int i);

int		sh_export(t_data *data, int i);
int		sh_export_str(t_data *data, char *str, int n);
int		sh_cd(t_data *data, int index_cmd);
int		sh_unset(t_data *data, int index_cmd);
int		sh_exit(t_data *data, int index_cmd);
int		sh_pwd(t_data *data, int index_cmd);

void	sigint_ctrl_c_handler(int signum);
void	sigquit_ctrl_slash_handler(int signum);
void	sigexit_ctrl_d_handler(t_data *data);

int		sh_init(t_data *data, int ac, char **av, char **env);
int		sh_init_cmd(t_cmd ***cmd, int n);
void	sh_reset_t_data(t_data *data);
int		sh_update_envp(t_data *data, int i);

int		sh_parse_cmds(t_data *data, int i, int m);
int		sh_recursive_parse_line2(t_data *data, char *line2, int i, t_cmd *cmd);
int		sh_extract_file(t_data *data, char *line2, int *i, t_list **list);
int		sh_extract_args(t_data *data, char *line2, int *i, t_list **list);
int		sh_make_absolute_path(char **app, t_data *data);
char	*sh_recursive_replace_dollar_clean(char *line, int i, t_data *data);

int		sh_syntax_check(t_data *data);
int		sh_bad_syntax(char *s, char c);
int		sh_recursive_check(char **line, int i);
int		sh_check_semicolon(char **line, int i);
int		sh_check_redirection(char **line, int i);
int		sh_is_back_escape(char *line, int i);
char	*sh_fix_quotations(char *line, char *newline, int ij[2]);
void	sh_memcpy(char *dest, char *src, int n);

#endif