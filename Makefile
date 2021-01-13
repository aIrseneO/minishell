#* ************************************************************************** *#
#*                                                                            *#
#*                                                        :::      ::::::::   *#
#*   Makefile                                           :+:      :+:    :+:   *#
#*                                                    +:+ +:+         +:+     *#
#*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        *#
#*                                                +#+#+#+#+#+   +#+           *#
#*   Created: 2020/11/30 15:21:51 by atemfack          #+#    #+#             *#
#*   Updated: 2021/01/13 04:59:04 by atemfack         ###   ########.fr       *#
#*                                                                            *#
#* ************************************************************************** *#

NAME = minishell

SRCS = main.c sh_signal_handler.c sh_error_free_exit.c \
	sh_init.c sh_init_utils.c  \
	sh_parse_input.c sh_parse_input_utils.c \
	sh_syntax_check.c sh_syntax_check_utils1.c sh_syntax_check_utils2.c \
	sh_execute.c sh_execute_utils.c \
	sh_run.c sh_run_export.c sh_run_unset.c

OBJS		= $(subst .c,.o,$(SRCS))
HEADER		= includes/minishell.h

SRCS_BIN	= echo.c env.c pwd.c
OBJS_BIN	= $(subst .c,.o,$(SRCS_BIN))
BIN_PATH	= bin

CFLAGS		= -Wall -Wextra -Werror -g -fsanitize=address

LIBFT		= libft.a

all:		$(NAME) bin

$(NAME):	$(OBJS)	$(LIBFT)
			$(CC) $(CFLAGS) -o $@ $^

%.o:		srcs/%.c $(HEADER)
			$(CC) -I includes -I libft -c $(CFLAGS) -o $@ $<

$(LIBFT):
			@$(MAKE) f --no-print-directory -C libft
			@/bin/mv -f libft/$(LIBFT) .

$(BIN_PATH)/%:	%.o $(LIBFT)
				$(CC) -I libft $(CFLAGS) -o $@ $^

bin:		$(BIN_PATH)/echo $(BIN_PATH)/env $(BIN_PATH)/pwd

clean:
			/bin/rm -f $(OBJS) $(OBJS_BIN)

fclean:		clean
			/bin/rm -f $(NAME) $(LIBFT) $(BIN_PATH)/*

re:			fclean all

f:			all clean

.PHONY:		all clean fclean re f
