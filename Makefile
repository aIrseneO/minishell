#* ************************************************************************** *#
#*                                                                            *#
#*                                                        :::      ::::::::   *#
#*   Makefile                                           :+:      :+:    :+:   *#
#*                                                    +:+ +:+         +:+     *#
#*   By: atemfack <marvin@42.fr>                    +#+  +:+       +#+        *#
#*                                                +#+#+#+#+#+   +#+           *#
#*   Created: 2020/11/30 15:21:51 by atemfack          #+#    #+#             *#
#*   Updated: 2020/11/30 17:20:01 by atemfack         ###   ########.fr       *#
#*                                                                            *#
#* ************************************************************************** *#

NAME					= minishell

LIBFT_NAME				= libft.a

LIBFT					= libft

all:					$(NAME)

$(NAME):
						@$(MAKE) -C $(LIBFT)
						cp $(LIBFT)/$(LIBFT_NAME) .




clean:
						@$(MAKE) -C $(LIBFT) fclean
						rm -rf $(LIBFT_NAME)


fclean:					clean
						

re:						fclean $(NAME)

f:						all clean

.PHONY:					all clean fclean re f
