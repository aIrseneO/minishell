# Copyright (c) 2020 Arsene Temfack
#
# SPDX-License-Identifier: MIT

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

SRCS_BIN	= echo.c env.c pwd.c
OBJS_BIN	= $(subst .c,.o,$(SRCS_BIN))
BIN			= bin
BIN_PATH	= $(PWD)/$(BIN)
CFLAGS		= -Wall -Wextra -Werror -g -fsanitize=address

LIBFT		= libft.a

all:		libft path $(NAME) bin_dir bin

$(NAME):	$(OBJS)	$(LIBFT)
			$(CC) $(CFLAGS) -o $@ $^

%.o:		srcs/%.c $(HEADER)
			$(CC) -I includes -I libft -c $(CFLAGS) -o $@ $<

$(LIBFT):
			@$(MAKE) f --no-print-directory -C libft
			@/bin/mv -f libft/$(LIBFT) .

libft:
			@git clone https://github.com/airseneo/libft.git libft

$(BIN)/%:	%.o $(LIBFT)
				$(CC) -I libft $(CFLAGS) -o $@ $^

bin:		bin_dir $(BIN)/echo $(BIN)/env $(BIN)/pwd

path:
			@bash -c "if grep _BIN_PATH_ $(HEADER); \
			then /bin/sed -i \"s|_BIN_PATH_|$(BIN_PATH)|g\" $(HEADER); fi"

nopath:
			/bin/sed -i "s|PATH=.*|PATH=_BIN_PATH_\"|g" $(HEADER)

bin_dir:
			@bash -c "if [ ! -d ./bin ]; then mkdir bin; fi"

clean:
			/bin/rm -f $(OBJS) $(OBJS_BIN)

fclean:		clean
			/bin/rm -rf $(NAME) $(LIBFT) $(BIN) libft

re:			fclean all

f:			all clean

.PHONY:		all clean fclean re f
