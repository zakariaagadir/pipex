# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/16 18:00:00 by zmounji           #+#    #+#              #
#    Updated: 2025/01/19 04:16:56 by zmounji          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program names
PROG    = pipex
PROG_B  = pipex_bonus

# Source files
SRCS    = pipex.c utils.c
SRCS_B  = pipex_bonus.c utils.c utils_bonus.c \
get_next_line_utils_bonus.c get_next_line_bonus.c

# Object files
OBJS    = $(SRCS:.c=.o)
OBJS_B  = $(SRCS_B:.c=.o)

# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror

# Headers
HEADER  = -I.

# Libft
LIBFT   = libft.a

# Rules
all: $(PROG)

$(PROG): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -L. -lft -o $(PROG)
	@echo "\033[32mPipex Compiled!\033[0m"

bonus: $(PROG_B)

$(PROG_B): $(OBJS_B)
	@$(CC) $(CFLAGS) $(OBJS_B) -L. -lft -o $(PROG_B)
	@echo "\033[32mPipex Bonus Compiled!\033[0m"

clean:
	@rm -f $(OBJS) $(OBJS_B)
	@echo "\033[33mObject files removed!\033[0m"

fclean: clean
	@rm -f $(PROG) $(PROG_B)
	@echo "\033[31mExecutables removed!\033[0m"

re: fclean all

re_bonus: fclean bonus

.PHONY: all bonus clean fclean re re_bonus
