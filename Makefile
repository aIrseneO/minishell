################################################################################
#     Copyright (c) 2020 Arsene Temfack                                        #
#                                                                              #
#     SPDX-License-Identifier: MIT                                             #
################################################################################

NAME = minishell

SRCS = main.c sh_error_free_exit.c \
	sh_utils1.c sh_utils2.c \
	sh_signal_handler.c sh_signal_handler_utils.c \
	sh_init.c sh_init_utils.c \
	sh_parse_input.c sh_parse_input_utils1.c sh_parse_input_utils2.c \
	sh_parse_input_utils3.c \
	sh_syntax_check.c sh_syntax_check_utils1.c sh_syntax_check_utils2.c \
	sh_syntax_check_utils3.c sh_syntax_check_utils4.c \
	sh_execute.c sh_execute_utils1.c sh_execute_utils2.c \
	sh_run.c sh_cd.c sh_unset.c sh_exit.c sh_pwd.c \
	sh_export.c sh_export_utils.c

OBJS		= $(subst .c,.o,$(SRCS))
HEADER		= includes/minishell.h

CFLAGS		= -Wall -Wextra -Werror -g -fsanitize=address

BINDIR		= $(DESTDIR)/usr/bin

LIBFT		= libft.a

all:		libft $(NAME)

$(NAME):	$(OBJS)	$(LIBFT)
			$(CC) $(CFLAGS) -o $@ $^

%.o:		srcs/%.c $(HEADER)
			$(CC) -I includes -I libft -c $(CFLAGS) -o $@ $<

$(LIBFT):
			@$(MAKE) f --no-print-directory -C libft
			@/bin/mv -f libft/$(LIBFT) .

libft:
			@if [ ! -d libft ]; then \
				git clone https://github.com/airseneo/libft.git libft; \
			fi

install:	libft $(NAME)
			install -d $(BINDIR)
			install $(NAME) $(BINDIR)

test:		install
			@bash test/run-test.sh bash minishell ./test

clean:
			/bin/rm -f $(OBJS) $(OBJS_BIN)

fclean:		clean
			/bin/rm -rf $(NAME) $(LIBFT) $(BIN) libft test/result-test.cf

re:			fclean all

f:			all clean

.PHONY:		all libft  install clean fclean re f
