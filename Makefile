# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/19 19:01:24 by peda-cos          #+#    #+#              #
#    Updated: 2025/02/26 17:53:05 by peda-cos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
SRCS = main.c tokenizer.c parser.c executor.c builtins.c signal_handler.c utils.c
OBJS = $(SRCS:.c=.o)
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(LIBFT) $(OBJS) $(LDFLAGS)
	@echo "Compiling $(NAME)..."

$(LIBFT):
	@make -C $(LIBFT_DIR) all
	@echo "Libft build complete."

%.o: %.c minishell.h
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiling $<..."

clean:
	@rm -f $(OBJS)
	@make clean -C $(LIBFT_DIR)
	@echo "Cleaning objects..."

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@echo "Cleaning $(NAME)..."

re: fclean all

.PHONY: all clean fclean re
