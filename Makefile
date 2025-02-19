# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/19 19:01:24 by peda-cos          #+#    #+#              #
#    Updated: 2025/02/19 20:17:43 by peda-cos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -O2
LDFLAGS = -lreadline
SRCS = main.c
OBJS = $(SRCS:.c=.o)
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LDFLAGS)
	@echo "Compiling $(NAME)..."

$(LIBFT):
	@make -C $(LIBFT_DIR)
	@echo "Compiling $(LIBFT)..."

%.o: %.c minishell.h ./libft/libft.h
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiling $<..."

clean:
	@rm -f $(OBJS)
	@make clean -C $(LIBFT_DIR)
	@echo "Cleaning objects..."

fclean: clean
	@rm -f $(NAME)
	@rm fclean -C $(LIBFT_DIR)
	@echo "Cleaning $(NAME)..."

re: fclean all

.PHONY: all clean fclean re